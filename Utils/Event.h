#ifndef EVENT_H
#define EVENT_H

#include <cmath>
#include <exception>
#include <iostream>

#include "TChain.h"
#include "TFile.h"
#include "TString.h"


// Access the event content. Interfaces the TTree entries in the input ntuples.
//
// Author: Matthias Schroeder
//         matthias.schroeder@AT@desy.de
//         November 2013
class Event {
public:
  // Constructor. Expects name of the ntuple file and the maximum number 
  // of processed events. If numProcessed < 1, all events are processed.
  Event(const TString &fileName, int numProcessed);
  ~Event();

  // Return the maximum number of analysed events
  int nTotEvents() const { return nTotEvts_; }

  // Loads the content of the next event. Returns false if
  // no next event exists.
  bool loadNext();


  // Access to the event content

  // Event weight (for luminosity and PU profile)
  Float_t weight() const { return weight_; }

  // HT and MHT
  Float_t ht() const { return ht_; }
  Float_t mht() const { return mht_; }
  Float_t mhtPhi() const { return mhtPhi_; }
  // Number of HT jets
  UShort_t nJets() const { return nJets_; }
  // DeltaPhi between leading three MHT jets and MHT
  Float_t deltaPhi1() const { return deltaPhi1_; }
  Float_t deltaPhi2() const { return deltaPhi2_; }
  Float_t deltaPhi3() const { return deltaPhi3_; }

  // Transverse mass of leading isolated muon and MET
  Float_t mt() const { return mt_; }
  // MET for mt computation
  Float_t met() const { return met_; }
  Float_t metPhi() const { return metPhi_; }
  
  // Well-reconstructed jets
  UShort_t jetsN() const { return jetsN_; }   
  Float_t*   jetsPt() const { return jetsPt_; }  
  Float_t*   jetsEta() const { return jetsEta_; } 
  Float_t*   jetsPhi() const { return jetsPhi_; }
  Float_t*   jetsE() const { return jetsE_; }   

  // Well-reconstructed muons
  UShort_t muonsN() const { return muonsN_; }   
  Float_t*   muonsPt() const { return muonsPt_; }  
  Float_t*   muonsEta() const { return muonsEta_; } 
  Float_t*   muonsPhi() const { return muonsPhi_; }
  Float_t*   muonsE() const { return muonsE_; }   

  // Well-reconstructed and isolated muons
  UShort_t isoMuonsN() const { return isoMuonsN_; }   
  Float_t*   isoMuonsPt() const { return isoMuonsPt_; }  
  Float_t*   isoMuonsEta() const { return isoMuonsEta_; } 
  Float_t*   isoMuonsPhi() const { return isoMuonsPhi_; }
  Float_t*   isoMuonsE() const { return isoMuonsE_; }   

  // Well-reconstructed electrons
  UShort_t electronsN() const { return electronsN_; }   
  Float_t*   electronsPt() const { return electronsPt_; }  
  Float_t*   electronsEta() const { return electronsEta_; } 
  Float_t*   electronsPhi() const { return electronsPhi_; }
  Float_t*   electronsE() const { return electronsE_; }   

  // Well-reconstructed and isolated electrons
  UShort_t isoElectronsN() const { return isoElectronsN_; }   
  Float_t*   isoElectronsPt() const { return isoElectronsPt_; }  
  Float_t*   isoElectronsEta() const { return isoElectronsEta_; } 
  Float_t*   isoElectronsPhi() const { return isoElectronsPhi_; }
  Float_t*   isoElectronsE() const { return isoElectronsE_; }   

  // Generator-level decay information
  UShort_t flgW() const { return flgW_; }
  UShort_t flgTau() const { return flgTau_; }

  // Generatorl-level letpon from W decay
  Float_t genLeptonPt() const { return genLeptonPt_; }
  Float_t genLeptonEta() const { return genLeptonEta_; }
  Float_t genLeptonPhi() const { return genLeptonPhi_; }
  Float_t genLeptonE() const { return genLeptonE_; }
  Int_t genLeptonPdgId() const { return genLeptonPdgId_; }

  // Generator-level lepton from tau decay
  Float_t genLeptonFromTauPt() const { return genLeptonFromTauPt_; }
  Float_t genLeptonFromTauEta() const { return genLeptonFromTauEta_; }
  Float_t genLeptonFromTauPhi() const { return genLeptonFromTauPhi_; }
  Float_t genLeptonFromTauE() const { return genLeptonFromTauE_; }
  Int_t genLeptonFromTauPdgId() const { return genLeptonFromTauPdgId_; }



private:
  const unsigned int maxColSize_;
  int nTotEvts_;
  int currentEntry_;

  TChain* chain_;

  Float_t weight_;

  Float_t ht_;
  Float_t mht_;
  Float_t mhtPhi_;
  UShort_t nJets_;
  Float_t deltaPhi1_;
  Float_t deltaPhi2_;
  Float_t deltaPhi3_;
  Float_t mt_;
  Float_t met_;
  Float_t metPhi_;

  UShort_t jetsN_;   
  Float_t*   jetsPt_;  
  Float_t*   jetsEta_; 
  Float_t*   jetsPhi_;
  Float_t*   jetsE_;   

  UShort_t muonsN_;   
  Float_t*   muonsPt_;  
  Float_t*   muonsEta_; 
  Float_t*   muonsPhi_;
  Float_t*   muonsE_;   

  UShort_t isoMuonsN_;   
  Float_t*   isoMuonsPt_;  
  Float_t*   isoMuonsEta_; 
  Float_t*   isoMuonsPhi_;
  Float_t*   isoMuonsE_;   

  UShort_t electronsN_;   
  Float_t*   electronsPt_;  
  Float_t*   electronsEta_; 
  Float_t*   electronsPhi_;
  Float_t*   electronsE_;   

  UShort_t isoElectronsN_;   
  Float_t*   isoElectronsPt_;  
  Float_t*   isoElectronsEta_; 
  Float_t*   isoElectronsPhi_;
  Float_t*   isoElectronsE_;   

  Float_t genLeptonPt_;
  Float_t genLeptonEta_;
  Float_t genLeptonPhi_;
  Float_t genLeptonE_;
  Int_t genLeptonPdgId_;

  Float_t genLeptonFromTauPt_;
  Float_t genLeptonFromTauEta_;
  Float_t genLeptonFromTauPhi_;
  Float_t genLeptonFromTauE_;
  Int_t genLeptonFromTauPdgId_;

  UShort_t flgW_;
  UShort_t flgTau_;
};


Event::Event(const TString &fileName, int nEvtsToProcess)
  : maxColSize_(50), currentEntry_(-1) {

  std::cout << "Setting up event variables ... " << std::flush;

  jetsPt_  = new Float_t[maxColSize_];  
  jetsEta_ = new Float_t[maxColSize_]; 
  jetsPhi_ = new Float_t[maxColSize_];
  jetsE_   = new Float_t[maxColSize_];   

  muonsPt_  = new Float_t[maxColSize_];  
  muonsEta_ = new Float_t[maxColSize_]; 
  muonsPhi_ = new Float_t[maxColSize_];
  muonsE_   = new Float_t[maxColSize_];   

  isoMuonsPt_  = new Float_t[maxColSize_];  
  isoMuonsEta_ = new Float_t[maxColSize_]; 
  isoMuonsPhi_ = new Float_t[maxColSize_];
  isoMuonsE_   = new Float_t[maxColSize_];   

  electronsPt_  = new Float_t[maxColSize_];  
  electronsEta_ = new Float_t[maxColSize_]; 
  electronsPhi_ = new Float_t[maxColSize_];
  electronsE_   = new Float_t[maxColSize_];   

  isoElectronsPt_  = new Float_t[maxColSize_];  
  isoElectronsEta_ = new Float_t[maxColSize_]; 
  isoElectronsPhi_ = new Float_t[maxColSize_];
  isoElectronsE_   = new Float_t[maxColSize_];   
  
  chain_ = new TChain("dasTree/DASRA2Tree");
  chain_->Add(fileName);
  nTotEvts_ = ( nEvtsToProcess < 0 ) ? chain_->GetEntries() : std::min(nEvtsToProcess,static_cast<int>(chain_->GetEntries()));

  // Set the branches
  chain_->SetBranchAddress("Weight",&weight_);

  chain_->SetBranchAddress("HT",&ht_);
  chain_->SetBranchAddress("MHT",&mht_);
  chain_->SetBranchAddress("MHTPhi",&mhtPhi_);
  chain_->SetBranchAddress("NJets",&nJets_);
  chain_->SetBranchAddress("DeltaPhi1",&deltaPhi1_);
  chain_->SetBranchAddress("DeltaPhi2",&deltaPhi2_);
  chain_->SetBranchAddress("DeltaPhi3",&deltaPhi3_);
  chain_->SetBranchAddress("MT",&mt_);
  chain_->SetBranchAddress("MET",&met_);
  chain_->SetBranchAddress("METPhi",&metPhi_);

  chain_->SetBranchAddress("JetsNum",&jetsN_);
  chain_->SetBranchAddress("JetsPt",  jetsPt_);  
  chain_->SetBranchAddress("JetsEta", jetsEta_);
  chain_->SetBranchAddress("JetsPhi", jetsPhi_);
  chain_->SetBranchAddress("JetsE",   jetsE_);

  chain_->SetBranchAddress("MuonsNum",&muonsN_);
  chain_->SetBranchAddress("MuonsPt",  muonsPt_);  
  chain_->SetBranchAddress("MuonsEta", muonsEta_);
  chain_->SetBranchAddress("MuonsPhi", muonsPhi_);
  chain_->SetBranchAddress("MuonsE",   muonsE_);

  chain_->SetBranchAddress("IsoMuonsNum",&isoMuonsN_);
  chain_->SetBranchAddress("IsoMuonsPt",  isoMuonsPt_);  
  chain_->SetBranchAddress("IsoMuonsEta", isoMuonsEta_);
  chain_->SetBranchAddress("IsoMuonsPhi", isoMuonsPhi_);
  chain_->SetBranchAddress("IsoMuonsE",   isoMuonsE_);

  chain_->SetBranchAddress("ElecsNum",&electronsN_);
  chain_->SetBranchAddress("ElecsPt",  electronsPt_);  
  chain_->SetBranchAddress("ElecsEta", electronsEta_);
  chain_->SetBranchAddress("ElecsPhi", electronsPhi_);
  chain_->SetBranchAddress("ElecsE",   electronsE_);

  chain_->SetBranchAddress("IsoElecsNum",&isoElectronsN_);
  chain_->SetBranchAddress("IsoElecsPt",  isoElectronsPt_);  
  chain_->SetBranchAddress("IsoElecsEta", isoElectronsEta_);
  chain_->SetBranchAddress("IsoElecsPhi", isoElectronsPhi_);
  chain_->SetBranchAddress("IsoElecsE",   isoElectronsE_);

  if( chain_->GetListOfBranches()->FindObject("FlgW") ) {
    // In case of W+jets sampel
    chain_->SetBranchAddress("FlgW",&flgW_);
    chain_->SetBranchAddress("FlgTau",&flgTau_);
    
    chain_->SetBranchAddress("GenLeptonPt",  &genLeptonPt_);  
    chain_->SetBranchAddress("GenLeptonEta", &genLeptonEta_);
    chain_->SetBranchAddress("GenLeptonPhi", &genLeptonPhi_);
    chain_->SetBranchAddress("GenLeptonE",   &genLeptonE_);
    chain_->SetBranchAddress("GenLeptonPdgId",  &genLeptonPdgId_);
    
    chain_->SetBranchAddress("GenLeptonFromTauPt",  &genLeptonFromTauPt_);  
    chain_->SetBranchAddress("GenLeptonFromTauEta", &genLeptonFromTauEta_);
    chain_->SetBranchAddress("GenLeptonFromTauPhi", &genLeptonFromTauPhi_);
    chain_->SetBranchAddress("GenLeptonFromTauE",   &genLeptonFromTauE_);
    chain_->SetBranchAddress("GenLeptonFromTauPdgId",  &genLeptonFromTauPdgId_);
  }

  std::cout << "ok" << std::endl;
}


Event::~Event() {

  for(unsigned int i = 0; i < maxColSize_; ++i) {
    delete [] jetsPt_;  
    delete [] jetsEta_; 
    delete [] jetsPhi_;
    delete [] jetsPhi_;   
    delete [] muonsPt_;  
    delete [] muonsEta_; 
    delete [] muonsPhi_;
    delete [] muonsPhi_;   
    delete [] isoMuonsPt_;  
    delete [] isoMuonsEta_; 
    delete [] isoMuonsPhi_;
    delete [] isoMuonsPhi_;   
    delete [] electronsPt_;  
    delete [] electronsEta_; 
    delete [] electronsPhi_;
    delete [] electronsPhi_;   
    delete [] isoElectronsPt_;  
    delete [] isoElectronsEta_; 
    delete [] isoElectronsPhi_;
    delete [] isoElectronsPhi_;   
  }

  delete chain_;
}


bool Event::loadNext() {
  if( currentEntry_ < 0 ) {
    std::cout << "Processing " << nTotEvents() << " events" << std::endl;
  }
  ++currentEntry_;

  if( currentEntry_ == nTotEvts_ ) {

    return false;

  } else {

    if( currentEntry_ > 0 && currentEntry_%100000 == 0 ) {
      std::cout << "  Processed " << currentEntry_ << " events" << std::endl;
    }
  
    chain_->GetEntry(currentEntry_);
    if( jetsN_ > maxColSize_ ) {
      std::cerr << "\n\nERROR: number of read jets in entry " << currentEntry_ << " larger than array size" << std::endl;
      throw std::exception();
    }
    if( muonsN_ > maxColSize_ ) {
      std::cerr << "\n\nERROR: number of read muons in entry " << currentEntry_ << " larger than array size" << std::endl;
      throw std::exception();
    }
    if( isoMuonsN_ > maxColSize_ ) {
      std::cerr << "\n\nERROR: number of read iso-muons in entry " << currentEntry_ << " larger than array size" << std::endl;
      throw std::exception();
    }
    if( electronsN_ > maxColSize_ ) {
      std::cerr << "\n\nERROR: number of read electrons in entry " << currentEntry_ << " larger than array size: " << std::endl;
      throw std::exception();
    }
    if( isoElectronsN_ > maxColSize_ ) {
      std::cerr << "\n\nERROR: number of read iso-electrons in entry " << currentEntry_ << " larger than array size" << std::endl;
      throw std::exception();
    }

    return true;
  }
}

#endif
