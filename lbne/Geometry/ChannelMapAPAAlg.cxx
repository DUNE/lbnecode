////////////////////////////////////////////////////////////////////////
/// \file  ChannelMapAPAAlg.cxx
/// \brief Interface to algorithm class for a specific detector channel mapping
///
/// \version $Id:  $
/// \author  tylerdalion@gmail.com
////////////////////////////////////////////////////////////////////////

#include "lbne/Geometry/ChannelMapAPAAlg.h"
#include "Geometry/AuxDetGeo.h"
#include "Geometry/CryostatGeo.h"
#include "Geometry/TPCGeo.h"
#include "Geometry/PlaneGeo.h"
#include "Geometry/WireGeo.h"

#include "art/Framework/Services/Registry/ServiceHandle.h"
#include "messagefacility/MessageLogger/MessageLogger.h" 

namespace geo{

  //----------------------------------------------------------------------------
  ChannelMapAPAAlg::ChannelMapAPAAlg(fhicl::ParameterSet const& p)
    : fSorter(geo::GeoObjectSorterAPA(p))
  {
  }

  //----------------------------------------------------------------------------
  ChannelMapAPAAlg::~ChannelMapAPAAlg()
  {
  }

  //----------------------------------------------------------------------------
  void ChannelMapAPAAlg::Initialize( std::vector<geo::CryostatGeo*> & cgeo,
				     std::vector<geo::AuxDetGeo*>   & adgeo )
  {

    if(!fFirstChannelInThisPlane.empty() || !fFirstChannelInNextPlane.empty())
      {
	this->Uninitialize();
      }

    fNcryostat = cgeo.size();
    
    mf::LogInfo("ChannelMapAPAAlg") << "Sorting volumes...";

    fSorter.SortAuxDets(adgeo);
    fSorter.SortCryostats(cgeo);
    for(size_t c = 0; c < cgeo.size(); ++c) 
      cgeo[c]->SortSubVolumes(fSorter);

    mf::LogInfo("ChannelMapAPAAlg") << "Initializing channel map...";
      
    fNTPC.resize(fNcryostat);
    fFirstChannelInNextPlane.resize(1);  // Change 1 to Ncryostat if you want
    fFirstChannelInThisPlane.resize(1);  // to treat each APA uniquely,and do
					 // the same with the other resizes.
    fPlanesPerAPA = cgeo[0]->TPC(0).Nplanes();
    nAnchoredWires.resize(fPlanesPerAPA);
    fWiresInPlane.resize(fPlanesPerAPA);
    fFirstChannelInThisPlane[0].resize(1);  // remember FirstChannel vectors
    fFirstChannelInNextPlane[0].resize(1);  // for first APA only.
    fFirstChannelInThisPlane[0][0].resize(fPlanesPerAPA);  // Make room for info
    fFirstChannelInNextPlane[0][0].resize(fPlanesPerAPA);  // on each plane.
    fViews.clear();
    fPlaneIDs.clear();
    fTopChannel = 0;

    // Size some vectors and initialize the FirstChannel vectors.
    // If making FirstChannel's for every APA uniquely, they also
    // need to be sized here. Not necessary for now
    for(unsigned int cs = 0; cs != fNcryostat; ++cs){
      
      fNTPC[cs] = cgeo[cs]->NTPC();

    }// end sizing loop over cryostats

    // Find the number of wires anchored to the frame
    for(unsigned int p=0; p!=fPlanesPerAPA; ++p){

      fWiresInPlane[p] = cgeo[0]->TPC(0).Plane(p).Nwires();
      double xyz[3] = {0.};
      double xyz_next[3] = {0.};

      fViews.emplace(cgeo[0]->TPC(0).Plane(p).View());
      
      for(unsigned int w = 0; w != fWiresInPlane[p]; ++w){

	// for vertical planes
	if(cgeo[0]->TPC(0).Plane(p).View()==geo::kZ)   { 
	  nAnchoredWires[p] = fWiresInPlane[p];      
	  break;
	}

	cgeo[0]->TPC(0).Plane(p).Wire(w).GetCenter(xyz);
	cgeo[0]->TPC(0).Plane(p).Wire(w+1).GetCenter(xyz_next);

	if(xyz[2]==xyz_next[2]){
	  nAnchoredWires[p] = w-1;      
	  break;
	}
      }// end wire loop

    }// end plane loop

    static uint32_t CurrentChannel = 0;
   
    for(unsigned int PCount = 0; PCount != fPlanesPerAPA; ++PCount){

      fFirstChannelInThisPlane[0][0][PCount] = CurrentChannel;
      CurrentChannel = CurrentChannel + 2*nAnchoredWires[PCount];
      fFirstChannelInNextPlane[0][0][PCount] = CurrentChannel;

    }// end build loop over planes

    // Save the number of channels
    fChannelsPerAPA = fFirstChannelInNextPlane[0][0][fPlanesPerAPA-1];

    fNchannels = 0;
    for(size_t cs = 0; cs < fNcryostat; ++cs){
      fNchannels = fNchannels + fChannelsPerAPA*fNTPC[cs]/2;
    }

    //save data into fFirstWireCenterY and fFirstWireCenterZ
    fPlaneData.resize(fNcryostat);
    for (unsigned int cs=0; cs<fNcryostat; ++cs){
      fPlaneData[cs].resize(cgeo[cs]->NTPC());
      for (unsigned int tpc=0; tpc<cgeo[cs]->NTPC(); ++tpc){
        fPlaneData[cs][tpc].resize(cgeo[cs]->TPC(tpc).Nplanes());
        for (unsigned int plane=0; plane<cgeo[cs]->TPC(tpc).Nplanes(); ++plane){
          PlaneData_t& PlaneData = fPlaneData[cs][tpc][plane];
          const geo::PlaneGeo& thePlane = cgeo[cs]->TPC(tpc).Plane(plane);
          double xyz[3];
          fPlaneIDs.emplace(cs, tpc, plane);
          thePlane.Wire(0).GetCenter(xyz);
          PlaneData.fFirstWireCenterY = xyz[1];
          PlaneData.fFirstWireCenterZ = xyz[2];
          // we are interested in the ordering of wire numbers: we find that a
          // point is N wires left of a wire W: is that wire W + N or W - N?
          // In fact, for TPC #0 it is W + N for V and Z planes, W - N for U
          // plane; for TPC #0 it is W + N for V and Z planes, W - N for U
          PlaneData.fWireSortingInZ = thePlane.WireIDincreasesWithZ()? +1.: -1.;
        } // for plane
      } // for TPC
    } // for cryostat

    //initialize fWirePitch and fOrientation
    fWirePitch.resize(cgeo[0]->TPC(0).Nplanes());
    fOrientation.resize(cgeo[0]->TPC(0).Nplanes());
    fSinOrientation.resize(cgeo[0]->TPC(0).Nplanes());
    fCosOrientation.resize(cgeo[0]->TPC(0).Nplanes());

    for (unsigned int plane=0; plane<cgeo[0]->TPC(0).Nplanes(); plane++){
      fWirePitch[plane]=cgeo[0]->TPC(0).WirePitch(0,1,plane);
      fOrientation[plane]=cgeo[0]->TPC(0).Plane(plane).Wire(0).ThetaZ();
      fSinOrientation[plane] = sin(fOrientation[plane]);
      fCosOrientation[plane] = cos(fOrientation[plane]);
    }


    mf::LogVerbatim("ChannelMapAPAAlg") << "fNchannels = " << fNchannels ; 

    mf::LogVerbatim("ChannelMapAPAAlg") << "For all identical APA:" ; 
    mf::LogVerbatim("ChannelMapAPAAlg") << "Number of channels per APA = " << fChannelsPerAPA ; 

    mf::LogVerbatim("ChannelMapAPAAlg") << "Number of WireIDs in a U plane = " << fWiresInPlane[0] ;
    mf::LogVerbatim("ChannelMapAPAAlg") << "Number of WireIDs in a V plane = " << fWiresInPlane[1] ;
    mf::LogVerbatim("ChannelMapAPAAlg") << "Number of WireIDs in a Z Plane = " << fWiresInPlane[2] ;

    mf::LogVerbatim("ChannelMapAPAAlg") << "U channels per APA = " << 2*nAnchoredWires[0] ;
    mf::LogVerbatim("ChannelMapAPAAlg") << "V channels per APA = " << 2*nAnchoredWires[1] ;
    mf::LogVerbatim("ChannelMapAPAAlg") << "Z channels per APA side = " << nAnchoredWires[2] ;

    mf::LogVerbatim("ChannelMapAPAAlg") << "Pitch in U Plane = " << fWirePitch[0] ;
    mf::LogVerbatim("ChannelMapAPAAlg") << "Pitch in V Plane = " << fWirePitch[1] ;
    mf::LogVerbatim("ChannelMapAPAAlg") << "Pitch in Z Plane = " << fWirePitch[2] ;

    return;

  }
   
  //----------------------------------------------------------------------------
  void ChannelMapAPAAlg::Uninitialize()
  {

    std::vector< std::vector<std::vector<uint32_t> > >().swap(fFirstChannelInThisPlane);
    std::vector< std::vector<std::vector<uint32_t> > >().swap(fFirstChannelInNextPlane);

  }

  //----------------------------------------------------------------------------
  std::vector<geo::WireID> ChannelMapAPAAlg::ChannelToWire(uint32_t channel)  const
  {

    // first check if this channel ID is legal
    if(channel >= fNchannels )
      throw cet::exception("Geometry") << "ILLEGAL CHANNEL ID for channel " << channel << "\n";

    std::vector< WireID > AllSegments;
    
    static unsigned int cstat;
    static unsigned int tpc;
    static unsigned int plane;
    static unsigned int wireThisPlane;
    static unsigned int NextPlane;
    static unsigned int ThisPlane;
    
    uint32_t chan       = channel%fChannelsPerAPA;
    uint32_t pureAPAnum = std::floor( channel/fChannelsPerAPA );

    bool breakVariable = false;
    for(unsigned int planeloop = 0; planeloop != fPlanesPerAPA; ++planeloop){
	  
      NextPlane = fFirstChannelInNextPlane[0][0][planeloop];
      ThisPlane = fFirstChannelInThisPlane[0][0][planeloop];
	  
      if(chan < NextPlane){

	// fNTPC[0] works for now since there are the same number of TPCs per crostat
	cstat = std::floor( channel / ((fNTPC[0]/2)*fChannelsPerAPA) );
	tpc   = (2*pureAPAnum) % fNTPC[0];
	plane = planeloop;
	wireThisPlane  = chan - ThisPlane;
	    
	breakVariable = true;
	break;
      }
      if(breakVariable) break;	  
    }// end plane loop

    

    int WrapDirection = 1; // go from tpc to (tpc+1) or tpc to (tpc-1)

    // find the lowest wire
    uint32_t ChannelGroup = std::floor( wireThisPlane/nAnchoredWires[plane] );
    unsigned int bottomwire = wireThisPlane-ChannelGroup*nAnchoredWires[plane];
    
    if(ChannelGroup%2==1){
      tpc += 1;
      WrapDirection  = -1;	 
    }
    
    for(unsigned int SegCount = 0; SegCount != 50; ++SegCount){
      
      tpc += WrapDirection*(SegCount%2);
      geo::WireID CodeWire(cstat, tpc, plane, bottomwire + SegCount*nAnchoredWires[plane]);
      AllSegments.push_back(CodeWire);
      
      // reset the tcp variable so it doesnt "accumulate value"
      tpc -= WrapDirection*(SegCount%2);
      
      if( bottomwire + (SegCount+1)*nAnchoredWires[plane] > fWiresInPlane[plane]-1) break;
    }
    
    return AllSegments;
  }


  //----------------------------------------------------------------------------
  uint32_t ChannelMapAPAAlg::Nchannels() const
  {
    return fNchannels;
  }
  
  
  //----------------------------------------------------------------------------
  double ChannelMapAPAAlg::WireCoordinate(double YPos, double ZPos,
                                          unsigned int PlaneNo,
                                          unsigned int TPCNo,
                                          unsigned int cstat) const
  {
    // Returns the wire number corresponding to a (Y,Z) position in PlaneNo
    // with float precision.
    // Core code ripped from original NearestWireID() implementation
    
    const PlaneData_t& PlaneData = fPlaneData[cstat][TPCNo][PlaneNo];
    
    //get the orientation angle of a given plane and calculate the distance between first wire
    //and a point projected in the plane
//    const double rotate = (TPCNo % 2 == 1)? -1.: +1.;
    
    // the formula used here is geometric:
    // distance = delta_y cos(theta_z) + delta_z sin(theta_z)
    // with a correction for the orientation of the TPC:
    // odd TPCs have supplementary wire angle (pi-theta_z), changing cosine sign
    
    const bool bSuppl = (TPCNo % 2) == 1;
    float distance =
      -(YPos - PlaneData.fFirstWireCenterY) * (bSuppl? -1.: +1.) * fCosOrientation[PlaneNo]
      +(ZPos - PlaneData.fFirstWireCenterZ) * fSinOrientation[PlaneNo]
      ;
    
    // The sign of this formula is correct if the wire with larger ID is on the
    // "right" (intuitively, larger z; rigorously, smaller intercept)
    // than this one.
    // Of course, we are not always that lucky. fWireSortingInZ fixes our luck.

    return PlaneData.fWireSortingInZ * distance/fWirePitch[PlaneNo];
  } // ChannelMapAPAAlg::WireCoordinate()
  
  
  //----------------------------------------------------------------------------
  WireID ChannelMapAPAAlg::NearestWireID(const TVector3& xyz,
                                         unsigned int    plane,
                                         unsigned int    tpc,
                                         unsigned int    cryostat)     const
  {
    // add 0.5 to have the correct rounding
    int NearestWireNumber
      = int (0.5 + WireCoordinate(xyz.Y(), xyz.Z(), plane, tpc, cryostat));
    
    // If we are outside of the wireplane range, throw an exception
    // (this response maintains consistency with the previous
    // implementation based on geometry lookup)
    if(NearestWireNumber < 0 ||
       NearestWireNumber >= (int) fWiresInPlane[plane])
    {
      const int wireNumber = NearestWireNumber; // save for the output
      
      if(wireNumber < 0 ) NearestWireNumber = 0;
      else                NearestWireNumber = fWiresInPlane[plane] - 1;
    
    /*
      // comment in the following statement to throw an exception instead
      throw InvalidWireIDError("Geometry", wireNumber, NearestWireNumber)
        << "ChannelMapAPAAlg::NearestWireID(): can't Find Nearest Wire for position (" 
        << xyz.X() << "," << xyz.Y() << "," << xyz.Z() << ")"
        << " approx wire number # " << wireNumber
        << " (capped from " << NearestWireNumber << ")\n";
    */
    } // if invalid wire
    
    return { cryostat, tpc, plane, (unsigned int) NearestWireNumber };
  } // ChannelMapAPAAlg::NearestWireID()
  
  
  //----------------------------------------------------------------------------
  uint32_t ChannelMapAPAAlg::PlaneWireToChannel(unsigned int plane,
						unsigned int wire,
						unsigned int tpc,
						unsigned int cstat) const
  {
    unsigned int OtherSideWires = 0;

    uint32_t Channel = fFirstChannelInThisPlane[0][0][plane]; // start in very first APA.
    Channel += cstat*(fNTPC[cstat]/2)*fChannelsPerAPA;       // move channel to proper cstat.
    Channel += std::floor( tpc/2 )*fChannelsPerAPA;		  // move channel to proper APA.
    OtherSideWires += (tpc%2)*nAnchoredWires[plane];	          // get number of wires on the first
								  // side of the APA if starting
								  // on the other side TPC.


    // Lastly, account for the fact that channel number while moving up wire number in one
    // plane resets after 2 times the number of wires anchored -- one for each APA side.
    // At the same time, OtherSideWires accounts for the fact that if a channel starts on 
    // the other side, it is offset by the number of wires on the first side.
    Channel += (OtherSideWires + wire)%(2*nAnchoredWires[plane]);
    
    return Channel;

  }

  //----------------------------------------------------------------------------
  SigType_t ChannelMapAPAAlg::SignalType( uint32_t const channel )  const
  {
    uint32_t chan = channel % fChannelsPerAPA;
    SigType_t sigt = kInduction;

    // instead of calling channel to wire, we can make use of the way we 
    // up channels among APAs;
    // the first two planes are induction, and the last one is collection
    if(       chan <  fFirstChannelInThisPlane[0][0][2]     ){ sigt = kInduction;  }
    else if( (chan >= fFirstChannelInThisPlane[0][0][2]) &&
             (chan <  fFirstChannelInNextPlane[0][0][2])    ){ sigt = kCollection; }
    else{    mf::LogWarning("BadChannelSignalType") << "Channel " << channel 
						    << " (" << chan << ") not given signal type." << std::endl;         }
  
    return sigt;
  }

  //----------------------------------------------------------------------------
  View_t ChannelMapAPAAlg::View( uint32_t const channel )  const
  {
    uint32_t chan = channel % fChannelsPerAPA;
    View_t view = geo::kU;

    // instead of calling channel to wire, we can make use of the way we 
    // up channels among APAs;
    // Plane 0: U, Plane 1: V, Plane 2: W
    if(       chan <  fFirstChannelInNextPlane[0][0][0]     ){ view = geo::kU; }
    else if( (chan >= fFirstChannelInThisPlane[0][0][1]) &&
             (chan <  fFirstChannelInNextPlane[0][0][1])    ){ view = geo::kV; }
    else if( (chan >= fFirstChannelInThisPlane[0][0][2]) &&
             (chan <  fFirstChannelInNextPlane[0][0][2])    ){ view = geo::kZ; }
    else{    mf::LogWarning("BadChannelViewType") << "Channel " << channel 
						  << " (" << chan << ") not given view type.";  }
    
    return view;
  }  

  //----------------------------------------------------------------------------
  std::set<View_t> const& ChannelMapAPAAlg::Views() const
  {
    return fViews;
  }

  //----------------------------------------------------------------------------
  std::set<PlaneID> const& ChannelMapAPAAlg::PlaneIDs() const
  {
    return fPlaneIDs;
  }

  //----------------------------------------------------------------------------
  unsigned int ChannelMapAPAAlg::NOpChannels(unsigned int NOpDets) const
  {
    return 12*NOpDets;
  }

  //----------------------------------------------------------------------------
  unsigned int ChannelMapAPAAlg::NOpHardwareChannels(unsigned int opDet) const
  {
    return 12;
  }

  //----------------------------------------------------------------------------
  unsigned int ChannelMapAPAAlg::OpChannel(unsigned int detNum, unsigned int channel) const
  {
    unsigned int uniqueChannel = (detNum * 12) + channel;
    return uniqueChannel;
  }

  //----------------------------------------------------------------------------
  unsigned int ChannelMapAPAAlg::OpDetFromOpChannel(unsigned int opChannel) const
  {
    unsigned int detectorNum = (unsigned int) opChannel / 12;
    return detectorNum;
  }

  //----------------------------------------------------------------------------
  unsigned int ChannelMapAPAAlg::HardwareChannelFromOpChannel(unsigned int opChannel) const
  {
    unsigned int channel = opChannel % 12;
    return channel;
  }

} // namespace
