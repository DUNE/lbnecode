////////////////////////////////////////////////////////////////////////
//
// CalWireLBNE10kt class
//
// brebel@fnal.gov
//
// 11-3-09 Pulled all FFT code out and put into Utilitiess/LArFFT
//
////////////////////////////////////////////////////////////////////////

// C/C++ standard libraries
#include <string>
#include <vector>
#include <utility> // std::move()
#include <memory> // std::unique_ptr<>

// ROOT libraries
#include "TComplex.h"

// framework libraries
#include "cetlib/exception.h"
#include "cetlib/search_path.h"
#include "fhiclcpp/ParameterSet.h" 
#include "messagefacility/MessageLogger/MessageLogger.h" 
#include "art/Framework/Core/ModuleMacros.h" 
#include "art/Framework/Core/EDProducer.h"
#include "art/Framework/Principal/Event.h" 
#include "art/Framework/Principal/Handle.h" 
#include "art/Persistency/Common/Ptr.h" 
#include "art/Framework/Services/Registry/ServiceHandle.h" 

// LArSoft libraries
#include "SimpleTypesAndConstants/RawTypes.h" // raw::ChannelID_t
#include "Geometry/Geometry.h"
#include "Filters/ChannelFilter.h"
#include "RawData/RawDigit.h"
#include "RawData/raw.h"
#include "RecoBase/Wire.h"
#include "RecoBaseArt/WireCreator.h"
#include "Utilities/LArFFT.h"
#include "Utilities/AssociationUtil.h"
#include "lbne/Utilities/SignalShapingServiceLBNE10kt.h"


///creation of calibrated signals on wires
namespace caldata {

  class CalWireLBNE10kt : public art::EDProducer {

  public:
    
    // create calibrated signals on wires. this class runs 
    // an fft to remove the electronics shaping.     
    explicit CalWireLBNE10kt(fhicl::ParameterSet const& pset); 
    virtual ~CalWireLBNE10kt();
    
    void produce(art::Event& evt); 
    void beginJob();
    void endJob();
    void reconfigure(fhicl::ParameterSet const& p);
 
  private:
    
    int          fDataSize;          ///< size of raw data on one wire
    int          fPostsample;        ///< number of postsample bins
    int          fDoBaselineSub;        ///< number of postsample bins
    std::string  fDigitModuleLabel;  ///< module that made digits
                                                       ///< constants
    std::string  fSpillName;  ///< nominal spill is an empty string
                              ///< it is set by the DigitModuleLabel
                              ///< ex.:  "daq:preSpill" for prespill data
    unsigned short fPreROIPad; ///< ROI padding
    unsigned short fPostROIPad; ///< ROI padding
    void SubtractBaseline(std::vector<float>& holder);
  protected: 
    
  }; // class CalWireLBNE10kt

  DEFINE_ART_MODULE(CalWireLBNE10kt)
  
  //-------------------------------------------------
  CalWireLBNE10kt::CalWireLBNE10kt(fhicl::ParameterSet const& pset)
  {
    this->reconfigure(pset);

    produces< std::vector<recob::Wire> >(fSpillName);
    produces<art::Assns<raw::RawDigit, recob::Wire>>(fSpillName);
  }
  
  //-------------------------------------------------
  CalWireLBNE10kt::~CalWireLBNE10kt()
  {
  }

  //////////////////////////////////////////////////////
  void CalWireLBNE10kt::reconfigure(fhicl::ParameterSet const& p)
  {
    std::vector<unsigned short> uin;    std::vector<unsigned short> vin;
    std::vector<unsigned short> zin;

    fDigitModuleLabel = p.get< std::string >("DigitModuleLabel", "daq");
    fPostsample       = p.get< int >        ("PostsampleBins");
    fDoBaselineSub    = p.get< bool >       ("DoBaselineSub");
    uin               = p.get< std::vector<unsigned short> >   ("PlaneROIPad");
    
     // put the ROI pad sizes into more convenient vectors
    fPreROIPad  = uin[0];
    fPostROIPad = uin[1];
    fSpillName.clear();
    
    size_t pos = fDigitModuleLabel.find(":");
    if( pos!=std::string::npos ) {
      fSpillName = fDigitModuleLabel.substr( pos+1 );
      fDigitModuleLabel = fDigitModuleLabel.substr( 0, pos );
    }
    
  }

  //-------------------------------------------------
  void CalWireLBNE10kt::beginJob()
  {  
  }

  //////////////////////////////////////////////////////
  void CalWireLBNE10kt::endJob()
  {  
  }
  
  //////////////////////////////////////////////////////
  void CalWireLBNE10kt::produce(art::Event& evt)
  {      
    // get the geometry
    art::ServiceHandle<geo::Geometry> geom;

    // get the FFT service to have access to the FFT size
    art::ServiceHandle<util::LArFFT> fFFT;
    int transformSize = fFFT->FFTSize();

    // Get signal shaping service.
    art::ServiceHandle<util::SignalShapingServiceLBNE10kt> sss;
    double DeconNorm = sss->GetDeconNorm();
    
    // make a collection of Wires
    std::unique_ptr<std::vector<recob::Wire> > wirecol(new std::vector<recob::Wire>);
    // ... and an association set
    std::unique_ptr<art::Assns<raw::RawDigit,recob::Wire> > WireDigitAssn
      (new art::Assns<raw::RawDigit,recob::Wire>);
    
    // Read in the digit List object(s). 
    art::Handle< std::vector<raw::RawDigit> > digitVecHandle;
    evt.getByLabel(fDigitModuleLabel, fSpillName, digitVecHandle);

    if (!digitVecHandle->size())  return;
    mf::LogInfo("CalWireLBNE10kt") << "CalWireLBNE10kt:: digitVecHandle size is " << digitVecHandle->size();

    // Use the handle to get a particular (0th) element of collection.
    art::Ptr<raw::RawDigit> digitVec0(digitVecHandle, 0);
        
    if (digitVec0->Compression() != raw::kZeroSuppression) {
      throw art::Exception(art::errors::UnimplementedFeature)
	<< "CalGausHFLBNE only supports zero-suppressed raw digit input!";
    } // if



    unsigned int dataSize = digitVec0->Samples(); //size of raw data vectors
    
    raw::ChannelID_t channel = raw::InvalidChannelID; // channel number
    unsigned int bin(0);     // time bin loop variable
    
    filter::ChannelFilter *chanFilt = new filter::ChannelFilter();  

    std::vector<float> holder;                // holds signal data
    std::vector<short> rawadc(transformSize);  // vector holding uncompressed adc values
    std::vector<TComplex> freqHolder(transformSize+1); // temporary frequency data
    
    // loop over all wires    
    wirecol->reserve(digitVecHandle->size());
    for(size_t rdIter = 0; rdIter < digitVecHandle->size(); ++rdIter){ // ++ move
      holder.clear();
      
      // get the reference to the current raw::RawDigit
      art::Ptr<raw::RawDigit> digitVec(digitVecHandle, rdIter);
      channel = digitVec->Channel();

      // skip bad channels
      if(!chanFilt->BadChannel(channel)) {
	holder.resize(transformSize);
	
	// uncompress the data
	raw::Uncompress(digitVec->ADCs(), rawadc, digitVec->Compression());
	
	// loop over all adc values and subtract the pedestal
	for(bin = 0; bin < dataSize; ++bin) 
	  holder[bin]=(rawadc[bin]-digitVec->GetPedestal());
	//Xin fill the remaining bin with data
	for (bin = dataSize;bin<holder.size();bin++){
	  holder[bin] = (rawadc[bin-dataSize]-digitVec->GetPedestal());
	}

	// Do deconvolution.
	sss->Deconvolute(channel, holder);
	for(bin = 0; bin < holder.size(); ++bin) holder[bin]=holder[bin]/DeconNorm;
      } // end if not a bad channel 
      
      holder.resize(dataSize,1e-5);

      //This restores the DC component to signal removed by the deconvolution.
      if(fPostsample) {
        double average=0.0;
	for(bin=0; bin < (unsigned int)fPostsample; ++bin) 
	  average+=holder[holder.size()-1-bin]/(double)fPostsample;
        for(bin = 0; bin < holder.size(); ++bin) holder[bin]-=average;
      }  
       // adaptive baseline subtraction
      if(fDoBaselineSub) SubtractBaseline(holder);
      
      // work out the ROI 
      recob::Wire::RegionsOfInterest_t ROIVec;
      std::vector<std::pair<unsigned int, unsigned int>> holderInfo;
      std::vector<std::pair<unsigned int, unsigned int>> rois;
      
      double max = 0;
      double deconNoise = sss->GetDeconNoise(channel);
      // find out all ROI
      unsigned int roiStart = 0;
      for(bin = 0; bin < dataSize; ++bin) {
	double SigVal = holder[bin];
	if (SigVal > max) max = SigVal;
	if(roiStart == 0) {
	  if (SigVal > 4*deconNoise) roiStart = bin; // 3 sigma above noise
	}else{
	  if (SigVal < deconNoise){
	    rois.push_back(std::make_pair(roiStart, bin));
	    roiStart = 0;
	  }
	}
      }
      if (roiStart!=0){
	rois.push_back(std::make_pair(roiStart, dataSize-1));
	roiStart = 0;
      }
      
      // pad them
      // if (channel==512){
      // 	for (bin = 0; bin< holder.size();++bin){
      // 	  if (fabs(holder[bin]) > 2)
      // 	      std::cout << "Xin1: " << holder[bin] << std::endl;
      // 	}
      // }
      //std::cout << "Xin: "  << max << " "<< channel << " " << deconNoise << " " << rois.size() << std::endl;

      if(rois.size() == 0) continue;
      holderInfo.clear();
      for(unsigned int ii = 0; ii < rois.size(); ++ii) {
	// low ROI end
	int low = rois[ii].first - fPreROIPad;
	if(low < 0) low = 0;
	rois[ii].first = low;
	// high ROI end
	unsigned int high = rois[ii].second + fPostROIPad;
	if(high >= dataSize) high = dataSize-1;
	rois[ii].second = high;
	
      }
      // merge them
      if(rois.size() >= 1) {
	// temporary vector for merged ROIs
		
	for (unsigned int ii = 0; ii<rois.size();ii++){
	  unsigned int roiStart = rois[ii].first;
	  unsigned int roiEnd = rois[ii].second;
	 	  
	  int flag1 = 1;
	  unsigned int jj=ii+1;
	  while(flag1){	
	    if (jj<rois.size()){
	      if(rois[jj].first <= roiEnd  ) {
		roiEnd = rois[jj].second;
		ii = jj;
		jj = ii+1;
	      }else{
		flag1 = 0;
	      }
	    }else{
	      flag1 = 0;
	    }
	  }
	  std::vector<float> sigTemp;
	  for(unsigned int kk = roiStart; kk < roiEnd; ++kk) {
	    sigTemp.push_back(holder[kk]);
	  } // jj
	  //	  std::cout << "Xin: " << roiStart << std::endl;
	  ROIVec.add_range(roiStart, std::move(sigTemp));
	  //trois.push_back(std::make_pair(roiStart,roiEnd));	    
	}
      }// else{
      // 	unsigned int roiStart = rois[0].first;
      // 	unsigned int roiEnd = rois[0].second;
      // 	std::vector<float> sigTemp;
      // 	  for(unsigned int kk = roiStart; kk < roiEnd; ++kk) {
      // 	    sigTemp.push_back(holder[kk]);
      // 	  } // jj
      // 	  //	  std::cout << "Xin: " << roiStart << std::endl;
      // 	  ROIVec.add_range(roiStart, std::move(sigTemp));
      // }
      
      // save them
      wirecol->push_back(recob::WireCreator(std::move(ROIVec),*digitVec).move());




      // Make a single ROI that spans the entire data size
      //wirecol->push_back(recob::WireCreator(holder,*digitVec).move());
      // add an association between the last object in wirecol
      // (that we just inserted) and digitVec
      if (!util::CreateAssn(*this, evt, *wirecol, digitVec, *WireDigitAssn, fSpillName)) {
        throw art::Exception(art::errors::InsertFailure)
          << "Can't associate wire #" << (wirecol->size() - 1)
          << " with raw digit #" << digitVec.key();
      } // if failed to add association
    }
    
    if(wirecol->size() == 0)
      mf::LogWarning("CalWireLBNE10kt") << "No wires made for this event.";

    evt.put(std::move(wirecol), fSpillName);
    evt.put(std::move(WireDigitAssn), fSpillName);


    delete chanFilt;
    return;
  }
  
  void CalWireLBNE10kt::SubtractBaseline(std::vector<float>& holder)
  {
    
    float min = 0,max=0;
    for (unsigned int bin = 0; bin < holder.size(); bin++){
      if (holder[bin] > max) max = holder[bin];
      if (holder[bin] < min) min = holder[bin];
    }
    int nbin = max - min;
    if (nbin!=0){
      TH1F *h1 = new TH1F("h1","h1",nbin,min,max);
      for (unsigned int bin = 0; bin < holder.size(); bin++){
	h1->Fill(holder[bin]);
      }
      float ped = h1->GetMaximum();
      float ave=0,ncount = 0;
      for (unsigned int bin = 0; bin < holder.size(); bin++){
	if (fabs(holder[bin]-ped)<2){
	  ave +=holder[bin];
	  ncount ++;
	}
      }
      if (ncount==0) ncount=1;
      ave = ave/ncount;
      for (unsigned int bin = 0; bin < holder.size(); bin++){
	holder[bin] -= ave;
      }
      h1->Delete();
    }
  }


} // end namespace caldata
