#ifndef HITFINDER35T_H
#define HITFINDER35T_H

////////////////////////////////////////////////////////////////////////
//
// HitFinder35t class
// 
// tjyang@fnal.gov
//  
// Based on Tom Junk's idea of 3-view matching
// HitFinder for 35t, input is undisambiguated hits, out is disambiguated hits
// Start from code written by talion@gmail.com
//
//
////////////////////////////////////////////////////////////////////////

extern "C" {
#include <sys/types.h>
#include <sys/stat.h>
}
#include <stdint.h>
#include <string>

// Framework includes
#include "art/Framework/Core/ModuleMacros.h" 
#include "art/Framework/Principal/Event.h"   
#include "art/Framework/Services/Optional/TFileService.h"
#include "art/Framework/Core/EDProducer.h" 


// LArSoft Includes
#include "Geometry/Geometry.h"
#include "Geometry/CryostatGeo.h"
#include "Geometry/TPCGeo.h"
#include "Geometry/PlaneGeo.h"
#include "RecoBase/Hit.h"
#include "RecoBase/Cluster.h"
#include "Utilities/AssociationUtil.h"
#include "Utilities/DetectorProperties.h"
#include "DisambigAlg35t.h"

// ROOT Includes 
#include "TH1D.h"
#include "TDecompSVD.h"
#include "TMath.h"
#include "TF1.h"
#include "TTree.h"
#include "TVectorD.h"
#include "TVector2.h"
#include "TVector3.h"

namespace lbne{
  class HitFinder35t : public art::EDProducer {
    
  public:
    
    explicit HitFinder35t(fhicl::ParameterSet const& pset); 
    
    void produce(art::Event& evt); 
    void beginJob(); 
    void endJob(); 
    void reconfigure(fhicl::ParameterSet const& p);                
    
    
  private:
    
    DisambigAlg35t    fDisambigAlg;
    art::ServiceHandle<geo::Geometry> fGeom;
    
    std::string fChanHitLabel;
    
    
  protected: 
    
    
  }; // class HitFinder35t
  
  
  //-------------------------------------------------
  //-------------------------------------------------
  HitFinder35t::HitFinder35t(fhicl::ParameterSet const& pset) :
  fDisambigAlg(pset.get< fhicl::ParameterSet >("DisambigAlg"))
  {
    this->reconfigure(pset);
    produces< std::vector<recob::Hit> >();
  }
  
  
  //-------------------------------------------------
  //-------------------------------------------------
  void HitFinder35t::reconfigure(fhicl::ParameterSet const& p)
  {
    
    fChanHitLabel =  p.get< std::string >("ChanHitLabel");
    
  }  
  
  //-------------------------------------------------
  //-------------------------------------------------
  void HitFinder35t::beginJob()
  {
    
  }
  
  //-------------------------------------------------
  //-------------------------------------------------
  void HitFinder35t::endJob()
  {
    
  }
  

  //-------------------------------------------------
  void HitFinder35t::produce(art::Event& evt)
  {
    
    std::unique_ptr<std::vector<recob::Hit> > hcol(new std::vector<recob::Hit>);
    art::Handle< std::vector<recob::Hit> > ChannelHits;
    evt.getByLabel(fChanHitLabel, ChannelHits);
    
    // Make unambiguous collection hits
    std::vector< art::Ptr<recob::Hit> >  ChHits;
    art::fill_ptr_vector(ChHits, ChannelHits);
    for( size_t h = 0; h < ChHits.size(); h++ ){
      if( ChHits[h]->View() != geo::kZ ) continue;
      art::Ptr<recob::Wire> wire = ChHits[h]->Wire();
      recob::Hit WidHit( wire, 			     ChHits[h]->WireID(),
			 ChHits[h]->StartTime(),       ChHits[h]->SigmaStartTime(),
			 ChHits[h]->EndTime(),         ChHits[h]->SigmaEndTime(),
			 ChHits[h]->PeakTime(),        ChHits[h]->SigmaPeakTime(),
			 ChHits[h]->Charge(),          ChHits[h]->SigmaCharge(),                
			 ChHits[h]->Charge(true),      ChHits[h]->SigmaCharge(true),
			 ChHits[h]->Multiplicity(),    ChHits[h]->GoodnessOfFit() );
      hcol->push_back(WidHit);
    }
    
    // Run alg on all APAs
    fDisambigAlg.RunDisambig(ChHits);
    
    
    for( size_t t=0; t < fDisambigAlg.fDisambigHits.size(); t++ ){
      art::Ptr<recob::Hit>  hit = fDisambigAlg.fDisambigHits[t].first;
      geo::WireID           wid = fDisambigAlg.fDisambigHits[t].second;
      art::Ptr<recob::Wire> wire = hit->Wire();
      recob::Hit WidHit( wire, 			     wid,
			 hit->StartTime(),     	     hit->SigmaStartTime(),
			 hit->EndTime(),       	     hit->SigmaEndTime(),
			 hit->PeakTime(),      	     hit->SigmaPeakTime(),
			 hit->Charge(),        	     hit->SigmaCharge(),                
			 hit->Charge(true),   	     hit->SigmaCharge(true),
			 hit->Multiplicity(),  	     hit->GoodnessOfFit() );
      hcol->push_back(WidHit);
    }
    
    evt.put(std::move(hcol));  
    
  }
  
  
  DEFINE_ART_MODULE(HitFinder35t)
  
} // end of lbne namespace
#endif // HITFINDER35T_H
