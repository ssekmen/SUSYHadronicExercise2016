#ifndef EVENT_H
#define EVENT_H

#include <cmath>
#include <exception>
#include <iostream>

#include "TChain.h"
#include "TFile.h"
#include "TString.h"
#include "TLorentzVector.h"

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
  Float_t weight() const { return Weight; }

  // HT and MHT
  Float_t ht() const { return HT; }
  Float_t mht() const { return MHT; }
  //Float_t mhtPhi() const { return mhtPhi_; }
  // Number of HT jets
  UShort_t nJets() const { return Jets->size(); }
  // DeltaPhi between leading three MHT jets and MHT
  Float_t deltaPhi1() const { return DeltaPhi1; }
  Float_t deltaPhi2() const { return DeltaPhi2; }
  Float_t deltaPhi3() const { return DeltaPhi3; }
  Float_t deltaPhi4() const { return DeltaPhi4; }

  // Transverse mass of leading isolated muon and MET
  //KH Float_t mt() const { return mt_; }
  // MET for mt computation
  Float_t met() const { return METPt; }
  Float_t metPhi() const { return METPhi; }
  
  // Well-reconstructed jets
  UShort_t jetsN() const { return Jets->size(); }   
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

  //
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

  // Declaration of leaf types
  UInt_t          RunNum;
  UInt_t          LumiBlockNum;
  ULong64_t       EvtNum;
  std::vector<TLorentzVector> *bestPhoton;
  Int_t           BTags;
  Int_t           BTagsclean;
  Double_t        CaloMETPhi;
  Double_t        CaloMETPt;
  Double_t        CrossSection;
  Bool_t          CSCTightHaloFilter;
  Double_t        DeltaPhi1;
  Double_t        DeltaPhi1clean;
  Double_t        DeltaPhi2;
  Double_t        DeltaPhi2clean;
  Double_t        DeltaPhi3;
  Double_t        DeltaPhi3clean;
  Double_t        DeltaPhi4;
  Double_t        DeltaPhi4clean;
  Int_t           EcalDeadCellTriggerPrimitiveFilter;
  Int_t           eeBadScFilter;
  std::vector<int>     *ElectronCharge;
  vector<TLorentzVector> *Electrons;
  vector<double>  *GenDeltaPhi;
  vector<int>     *GenElec_GenElecFromTau;
  vector<double>  *GenElec_MT2Activity;
  vector<double>  *GenElec_RecoTrkAct;
  vector<double>  *GenElec_RecoTrkd3;
  vector<double>  *GenElec_RecoTrkIso;
  vector<TLorentzVector> *GenEls;
  Double_t        genHT;
  vector<TLorentzVector> *GenJets;
  Double_t        GenMETPhi;
  Double_t        GenMETPt;
  Double_t        GenMHT;
  Double_t        GenMHT_Phi;
  vector<double>  *GenMinDeltaPhiEta24;
  vector<double>  *GenMinDeltaPhiEta5;
  vector<int>     *GenMinDeltaPhiIndexEta24;
  vector<int>     *GenMinDeltaPhiIndexEta5;
  vector<int>     *GenMu_GenMuFromTau;
  vector<double>  *GenMu_MT2Activity;
  vector<double>  *GenMu_RecoTrkAct;
  vector<double>  *GenMu_RecoTrkd3;
  vector<double>  *GenMu_RecoTrkIso;
  vector<TLorentzVector> *GenMus;
  vector<TLorentzVector> *genParticles;
  vector<int>     *genParticles_PDGid;
  vector<int>     *GenTau_GenTauHad;
  vector<double>  *GenTau_MT2Activity;
  vector<double>  *GenTauLeadRecoTrkAct;
  vector<double>  *GenTauLeadRecoTrkd3;
  vector<double>  *GenTauLeadRecoTrkIso;
  vector<TLorentzVector> *GenTauLeadTrk;
  vector<TLorentzVector> *GenTauNu;
  vector<TLorentzVector> *GenTaus;
  Bool_t          HBHEIsoNoiseFilter;
  Bool_t          HBHENoiseFilter;
  Double_t        HT;
  Double_t        HTclean;
  vector<bool>    *HTJetsMask;
  Int_t           isoElectronTracks;
  vector<TLorentzVector> *IsolatedElectronTracksVeto;
  vector<TLorentzVector> *IsolatedMuonTracksVeto;
  vector<TLorentzVector> *IsolatedPionTracksVeto;
  Int_t           isoMuonTracks;
  Int_t           isoPionTracks;
  Bool_t          JetID;
  vector<TLorentzVector> *Jets;
  vector<double>  *Jets_bDiscriminatorCSV;
  vector<double>  *Jets_bDiscriminatorMVA;
  vector<double>  *Jets_chargedEmEnergyFraction;
  vector<double>  *Jets_chargedHadronEnergyFraction;
  vector<int>     *Jets_chargedHadronMultiplicity;
  vector<int>     *Jets_chargedMultiplicity;
  vector<int>     *Jets_electronMultiplicity;
  vector<int>     *Jets_hadronFlavor;
  vector<double>  *Jets_jetArea;
  vector<double>  *Jets_muonEnergyFraction;
  vector<int>     *Jets_muonMultiplicity;
  vector<double>  *Jets_neutralEmEnergyFraction;
  vector<double>  *Jets_neutralHadronEnergyFraction;
  vector<int>     *Jets_neutralHadronMultiplicity;
  vector<int>     *Jets_neutralMultiplicity;
  vector<int>     *Jets_partonFlavor;
  vector<double>  *Jets_photonEnergyFraction;
  vector<int>     *Jets_photonMultiplicity;
  Int_t           Leptons;
  Int_t           METFilters;
  Double_t        METPhi;
  Double_t        METPhiclean;
  vector<double>  *METPhiDown;
  vector<double>  *METPhiUp;
  Double_t        METPt;
  Double_t        METPtclean;
  vector<double>  *METPtDown;
  vector<double>  *METPtUp;
  Double_t        MHT;
  Double_t        MHT_Phi;
  Double_t        MHT_Phiclean;
  Double_t        MHTclean;
  vector<bool>    *MHTJetsMask;
  vector<string>  *minDeltaPhiNames;
  vector<int>     *MuonCharge;
  vector<TLorentzVector> *Muons;
  Int_t           nAllVertices;
  vector<TLorentzVector> *NeutrinoLorentzVector;
  vector<int>     *NeutrinoMotherPdg;
  vector<int>     *NeutrinoPdg;
  Int_t           NJets;
  Int_t           NJetsclean;
  Int_t           nTAPElectronTracks;
  Int_t           nTAPMuonTracks;
  Int_t           nTAPPionTracks;
  Double_t        NumEvents;
  Int_t           NumInteractions;
  Int_t           NumPhotons;
  Int_t           NVtx;
  vector<int>     *PDFids;
  vector<double>  *PDFweights;
  vector<double>  *photon_genMatched;
  vector<double>  *photon_hadTowOverEM;
  vector<double>  *photon_hasPixelSeed;
  vector<double>  *photon_isEB;
  vector<bool>    *photon_nonPrompt;
  vector<double>  *photon_passElectronVeto;
  vector<double>  *photon_pfChargedIso;
  vector<double>  *photon_pfChargedIsoRhoCorr;
  vector<double>  *photon_pfGammaIso;
  vector<double>  *photon_pfGammaIsoRhoCorr;
  vector<double>  *photon_pfNeutralIso;
  vector<double>  *photon_pfNeutralIsoRhoCorr;
  vector<double>  *photon_sigmaIetaIeta;
  vector<TLorentzVector> *photonCands;
  Double_t        puSysDown;
  Double_t        puSysUp;
  Double_t        puWeight;
  vector<double>  *RJetDeltaPhi;
  vector<double>  *RJetMinDeltaPhiEta24;
  vector<double>  *RJetMinDeltaPhiEta5;
  vector<int>     *RJetMinDeltaPhiIndexEta24;
  vector<int>     *RJetMinDeltaPhiIndexEta5;
  vector<double>  *ScaleWeights;
  vector<TLorentzVector> *selectedIDElectrons;
  vector<bool>    *selectedIDElectrons_mediumID;
  vector<double>  *selectedIDElectrons_MiniIso;
  vector<double>  *selectedIDElectrons_MT2Activity;
  vector<double>  *selectedIDElectrons_MTW;
  vector<bool>    *selectedIDIsoElectrons_mediumID;
  vector<double>  *selectedIDIsoElectrons_MT2Activity;
  vector<double>  *selectedIDIsoElectrons_MTW;
  vector<double>  *selectedIDIsoElectrons_PTW;
  vector<double>  *selectedIDIsoMuons_MT2Activity;
  vector<double>  *selectedIDIsoMuons_MTW;
  vector<double>  *selectedIDIsoMuons_PTW;
  vector<TLorentzVector> *selectedIDMuons;
  vector<double>  *selectedIDMuons_MiniIso;
  vector<double>  *selectedIDMuons_MT2Activity;
  vector<double>  *selectedIDMuons_MTW;
  vector<TLorentzVector> *slimJet;
  vector<bool>    *slimJet_slimJetID;
  vector<TLorentzVector> *slimJetJECdown;
  vector<bool>    *slimJetJECdown_slimJetID;
  vector<TLorentzVector> *slimJetJECup;
  vector<bool>    *slimJetJECup_slimJetID;
  Double_t        SusyLSPMass;
  Double_t        SusyMotherMass;
  Int_t           TagLeptonHighPT;
  vector<TLorentzVector> *TAPElectronTracks;
  vector<double>  *TAPElectronTracks_activity;
  vector<int>     *TAPElectronTracks_chg;
  vector<double>  *TAPElectronTracks_mT;
  vector<double>  *TAPElectronTracks_trkiso;
  vector<TLorentzVector> *TAPMuonTracks;
  vector<double>  *TAPMuonTracks_activity;
  vector<int>     *TAPMuonTracks_chg;
  vector<double>  *TAPMuonTracks_mT;
  vector<double>  *TAPMuonTracks_trkiso;
  vector<TLorentzVector> *TAPPionTracks;
  vector<double>  *TAPPionTracks_activity;
  vector<int>     *TAPPionTracks_chg;
  vector<double>  *TAPPionTracks_mT;
  vector<double>  *TAPPionTracks_trkiso;
  vector<TLorentzVector> *TauDecayCands;
  vector<int>     *TauDecayCands_momInd;
  vector<int>     *TauDecayCands_pdgID;
  vector<string>  *TriggerNames;
  vector<int>     *TriggerPass;
  vector<int>     *TriggerPrescales;
  Double_t        TrueNumInteractions;
  Double_t        Weight;
  Int_t           ZNum;
  vector<TLorentzVector> *Zp4;

  // List of branches
  TBranch        *b_RunNum;   //!
  TBranch        *b_LumiBlockNum;   //!
  TBranch        *b_EvtNum;   //!
  TBranch        *b_bestPhoton;   //!
  TBranch        *b_BTags;   //!
  TBranch        *b_BTagsclean;   //!
  TBranch        *b_CaloMETPhi;   //!
  TBranch        *b_CaloMETPt;   //!
  TBranch        *b_CrossSection;   //!
  TBranch        *b_CSCTightHaloFilter;   //!
  TBranch        *b_DeltaPhi1;   //!
  TBranch        *b_DeltaPhi1clean;   //!
  TBranch        *b_DeltaPhi2;   //!
  TBranch        *b_DeltaPhi2clean;   //!
  TBranch        *b_DeltaPhi3;   //!
  TBranch        *b_DeltaPhi3clean;   //!
  TBranch        *b_DeltaPhi4;   //!
  TBranch        *b_DeltaPhi4clean;   //!
  TBranch        *b_EcalDeadCellTriggerPrimitiveFilter;   //!
  TBranch        *b_eeBadScFilter;   //!
  TBranch        *b_ElectronCharge;   //!
  TBranch        *b_Electrons;   //!
  TBranch        *b_GenDeltaPhi;   //!
  TBranch        *b_GenElec_GenElecFromTau;   //!
  TBranch        *b_GenElec_MT2Activity;   //!
  TBranch        *b_GenElec_RecoTrkAct;   //!
  TBranch        *b_GenElec_RecoTrkd3;   //!
  TBranch        *b_GenElec_RecoTrkIso;   //!
  TBranch        *b_GenEls;   //!
  TBranch        *b_genHT;   //!
  TBranch        *b_GenJets;   //!
  TBranch        *b_GenMETPhi;   //!
  TBranch        *b_GenMETPt;   //!
  TBranch        *b_GenMHT;   //!
  TBranch        *b_GenMHT_Phi;   //!
  TBranch        *b_GenMinDeltaPhiEta24;   //!
  TBranch        *b_GenMinDeltaPhiEta5;   //!
  TBranch        *b_GenMinDeltaPhiIndexEta24;   //!
  TBranch        *b_GenMinDeltaPhiIndexEta5;   //!
  TBranch        *b_GenMu_GenMuFromTau;   //!
  TBranch        *b_GenMu_MT2Activity;   //!
  TBranch        *b_GenMu_RecoTrkAct;   //!
  TBranch        *b_GenMu_RecoTrkd3;   //!
  TBranch        *b_GenMu_RecoTrkIso;   //!
  TBranch        *b_GenMus;   //!
  TBranch        *b_genParticles;   //!
  TBranch        *b_genParticles_PDGid;   //!
  TBranch        *b_GenTau_GenTauHad;   //!
  TBranch        *b_GenTau_MT2Activity;   //!
  TBranch        *b_GenTauLeadRecoTrkAct;   //!
  TBranch        *b_GenTauLeadRecoTrkd3;   //!
  TBranch        *b_GenTauLeadRecoTrkIso;   //!
  TBranch        *b_GenTauLeadTrk;   //!
  TBranch        *b_GenTauNu;   //!
  TBranch        *b_GenTaus;   //!
  TBranch        *b_HBHEIsoNoiseFilter;   //!
  TBranch        *b_HBHENoiseFilter;   //!
  TBranch        *b_HT;   //!
  TBranch        *b_HTclean;   //!
  TBranch        *b_HTJetsMask;   //!
  TBranch        *b_isoElectronTracks;   //!
  TBranch        *b_IsolatedElectronTracksVeto;   //!
  TBranch        *b_IsolatedMuonTracksVeto;   //!
  TBranch        *b_IsolatedPionTracksVeto;   //!
  TBranch        *b_isoMuonTracks;   //!
  TBranch        *b_isoPionTracks;   //!
  TBranch        *b_JetID;   //!
  TBranch        *b_Jets;   //!
  TBranch        *b_Jets_bDiscriminatorCSV;   //!
  TBranch        *b_Jets_bDiscriminatorMVA;   //!
  TBranch        *b_Jets_chargedEmEnergyFraction;   //!
  TBranch        *b_Jets_chargedHadronEnergyFraction;   //!
  TBranch        *b_Jets_chargedHadronMultiplicity;   //!
  TBranch        *b_Jets_chargedMultiplicity;   //!
  TBranch        *b_Jets_electronMultiplicity;   //!
  TBranch        *b_Jets_hadronFlavor;   //!
  TBranch        *b_Jets_jetArea;   //!
  TBranch        *b_Jets_muonEnergyFraction;   //!
  TBranch        *b_Jets_muonMultiplicity;   //!
  TBranch        *b_Jets_neutralEmEnergyFraction;   //!
  TBranch        *b_Jets_neutralHadronEnergyFraction;   //!
  TBranch        *b_Jets_neutralHadronMultiplicity;   //!
  TBranch        *b_Jets_neutralMultiplicity;   //!
  TBranch        *b_Jets_partonFlavor;   //!
  TBranch        *b_Jets_photonEnergyFraction;   //!
  TBranch        *b_Jets_photonMultiplicity;   //!
  TBranch        *b_Leptons;   //!
  TBranch        *b_METFilters;   //!
  TBranch        *b_METPhi;   //!
  TBranch        *b_METPhiclean;   //!
  TBranch        *b_METPhiDown;   //!
  TBranch        *b_METPhiUp;   //!
  TBranch        *b_METPt;   //!
  TBranch        *b_METPtclean;   //!
  TBranch        *b_METPtDown;   //!
  TBranch        *b_METPtUp;   //!
  TBranch        *b_MHT;   //!
  TBranch        *b_MHT_Phi;   //!
  TBranch        *b_MHT_Phiclean;   //!
  TBranch        *b_MHTclean;   //!
  TBranch        *b_MHTJetsMask;   //!
  TBranch        *b_minDeltaPhiNames;   //!
  TBranch        *b_MuonCharge;   //!
  TBranch        *b_Muons;   //!
  TBranch        *b_nAllVertices;   //!
  TBranch        *b_NeutrinoLorentzVector;   //!
  TBranch        *b_NeutrinoMotherPdg;   //!
  TBranch        *b_NeutrinoPdg;   //!
  TBranch        *b_NJets;   //!
  TBranch        *b_NJetsclean;   //!
  TBranch        *b_nTAPElectronTracks;   //!
  TBranch        *b_nTAPMuonTracks;   //!
  TBranch        *b_nTAPPionTracks;   //!
  TBranch        *b_NumEvents;   //!
  TBranch        *b_NumInteractions;   //!
  TBranch        *b_NumPhotons;   //!
  TBranch        *b_NVtx;   //!
  TBranch        *b_PDFids;   //!
  TBranch        *b_PDFweights;   //!
  TBranch        *b_photon_genMatched;   //!
  TBranch        *b_photon_hadTowOverEM;   //!
  TBranch        *b_photon_hasPixelSeed;   //!
  TBranch        *b_photon_isEB;   //!
  TBranch        *b_photon_nonPrompt;   //!
  TBranch        *b_photon_passElectronVeto;   //!
  TBranch        *b_photon_pfChargedIso;   //!
  TBranch        *b_photon_pfChargedIsoRhoCorr;   //!
  TBranch        *b_photon_pfGammaIso;   //!
  TBranch        *b_photon_pfGammaIsoRhoCorr;   //!
  TBranch        *b_photon_pfNeutralIso;   //!
  TBranch        *b_photon_pfNeutralIsoRhoCorr;   //!
  TBranch        *b_photon_sigmaIetaIeta;   //!
  TBranch        *b_photonCands;   //!
  TBranch        *b_puSysDown;   //!
  TBranch        *b_puSysUp;   //!
  TBranch        *b_puWeight;   //!
  TBranch        *b_RJetDeltaPhi;   //!
  TBranch        *b_RJetMinDeltaPhiEta24;   //!
  TBranch        *b_RJetMinDeltaPhiEta5;   //!
  TBranch        *b_RJetMinDeltaPhiIndexEta24;   //!
  TBranch        *b_RJetMinDeltaPhiIndexEta5;   //!
  TBranch        *b_ScaleWeights;   //!
  TBranch        *b_selectedIDElectrons;   //!
  TBranch        *b_selectedIDElectrons_mediumID;   //!
  TBranch        *b_selectedIDElectrons_MiniIso;   //!
  TBranch        *b_selectedIDElectrons_MT2Activity;   //!
  TBranch        *b_selectedIDElectrons_MTW;   //!
  TBranch        *b_selectedIDIsoElectrons_mediumID;   //!
  TBranch        *b_selectedIDIsoElectrons_MT2Activity;   //!
  TBranch        *b_selectedIDIsoElectrons_MTW;   //!
  TBranch        *b_selectedIDIsoElectrons_PTW;   //!
  TBranch        *b_selectedIDIsoMuons_MT2Activity;   //!
  TBranch        *b_selectedIDIsoMuons_MTW;   //!
  TBranch        *b_selectedIDIsoMuons_PTW;   //!
  TBranch        *b_selectedIDMuons;   //!
  TBranch        *b_selectedIDMuons_MiniIso;   //!
  TBranch        *b_selectedIDMuons_MT2Activity;   //!
  TBranch        *b_selectedIDMuons_MTW;   //!
  TBranch        *b_slimJet;   //!
  TBranch        *b_slimJet_slimJetID;   //!
  TBranch        *b_slimJetJECdown;   //!
  TBranch        *b_slimJetJECdown_slimJetID;   //!
  TBranch        *b_slimJetJECup;   //!
  TBranch        *b_slimJetJECup_slimJetID;   //!
  TBranch        *b_SusyLSPMass;   //!
  TBranch        *b_SusyMotherMass;   //!
  TBranch        *b_TagLeptonHighPT;   //!
  TBranch        *b_TAPElectronTracks;   //!
  TBranch        *b_TAPElectronTracks_activity;   //!
  TBranch        *b_TAPElectronTracks_chg;   //!
  TBranch        *b_TAPElectronTracks_mT;   //!
  TBranch        *b_TAPElectronTracks_trkiso;   //!
  TBranch        *b_TAPMuonTracks;   //!
  TBranch        *b_TAPMuonTracks_activity;   //!
  TBranch        *b_TAPMuonTracks_chg;   //!
  TBranch        *b_TAPMuonTracks_mT;   //!
  TBranch        *b_TAPMuonTracks_trkiso;   //!
  TBranch        *b_TAPPionTracks;   //!
  TBranch        *b_TAPPionTracks_activity;   //!
  TBranch        *b_TAPPionTracks_chg;   //!
  TBranch        *b_TAPPionTracks_mT;   //!
  TBranch        *b_TAPPionTracks_trkiso;   //!
  TBranch        *b_TauDecayCands;   //!
  TBranch        *b_TauDecayCands_momInd;   //!
  TBranch        *b_TauDecayCands_pdgID;   //!
  TBranch        *b_TriggerNames;   //!
  TBranch        *b_TriggerPass;   //!
  TBranch        *b_TriggerPrescales;   //!
  TBranch        *b_TrueNumInteractions;   //!
  TBranch        *b_Weight;   //!
  TBranch        *b_ZNum;   //!
  TBranch        *b_Zp4;   //!

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
  /*
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
  */
  chain_->SetBranchAddress("RunNum", &RunNum, &b_RunNum);
  chain_->SetBranchAddress("LumiBlockNum", &LumiBlockNum, &b_LumiBlockNum);
  chain_->SetBranchAddress("EvtNum", &EvtNum, &b_EvtNum);
  chain_->SetBranchAddress("bestPhoton", &bestPhoton, &b_bestPhoton);
  chain_->SetBranchAddress("BTags", &BTags, &b_BTags);
  chain_->SetBranchAddress("BTagsclean", &BTagsclean, &b_BTagsclean);
  chain_->SetBranchAddress("CaloMETPhi", &CaloMETPhi, &b_CaloMETPhi);
  chain_->SetBranchAddress("CaloMETPt", &CaloMETPt, &b_CaloMETPt);
  chain_->SetBranchAddress("CrossSection", &CrossSection, &b_CrossSection);
  chain_->SetBranchAddress("CSCTightHaloFilter", &CSCTightHaloFilter, &b_CSCTightHaloFilter);
  chain_->SetBranchAddress("DeltaPhi1", &DeltaPhi1, &b_DeltaPhi1);
  chain_->SetBranchAddress("DeltaPhi1clean", &DeltaPhi1clean, &b_DeltaPhi1clean);
  chain_->SetBranchAddress("DeltaPhi2", &DeltaPhi2, &b_DeltaPhi2);
  chain_->SetBranchAddress("DeltaPhi2clean", &DeltaPhi2clean, &b_DeltaPhi2clean);
  chain_->SetBranchAddress("DeltaPhi3", &DeltaPhi3, &b_DeltaPhi3);
  chain_->SetBranchAddress("DeltaPhi3clean", &DeltaPhi3clean, &b_DeltaPhi3clean);
  chain_->SetBranchAddress("DeltaPhi4", &DeltaPhi4, &b_DeltaPhi4);
  chain_->SetBranchAddress("DeltaPhi4clean", &DeltaPhi4clean, &b_DeltaPhi4clean);
  chain_->SetBranchAddress("EcalDeadCellTriggerPrimitiveFilter", &EcalDeadCellTriggerPrimitiveFilter, &b_EcalDeadCellTriggerPrimitiveFilter);
  chain_->SetBranchAddress("eeBadScFilter", &eeBadScFilter, &b_eeBadScFilter);
  chain_->SetBranchAddress("ElectronCharge", &ElectronCharge, &b_ElectronCharge);
  chain_->SetBranchAddress("Electrons", &Electrons, &b_Electrons);
  chain_->SetBranchAddress("GenDeltaPhi", &GenDeltaPhi, &b_GenDeltaPhi);
  chain_->SetBranchAddress("GenElec_GenElecFromTau", &GenElec_GenElecFromTau, &b_GenElec_GenElecFromTau);
  chain_->SetBranchAddress("GenElec_MT2Activity", &GenElec_MT2Activity, &b_GenElec_MT2Activity);
  chain_->SetBranchAddress("GenElec_RecoTrkAct", &GenElec_RecoTrkAct, &b_GenElec_RecoTrkAct);
  chain_->SetBranchAddress("GenElec_RecoTrkd3", &GenElec_RecoTrkd3, &b_GenElec_RecoTrkd3);
  chain_->SetBranchAddress("GenElec_RecoTrkIso", &GenElec_RecoTrkIso, &b_GenElec_RecoTrkIso);
  chain_->SetBranchAddress("GenEls", &GenEls, &b_GenEls);
  chain_->SetBranchAddress("genHT", &genHT, &b_genHT);
  chain_->SetBranchAddress("GenJets", &GenJets, &b_GenJets);
  chain_->SetBranchAddress("GenMETPhi", &GenMETPhi, &b_GenMETPhi);
  chain_->SetBranchAddress("GenMETPt", &GenMETPt, &b_GenMETPt);
  chain_->SetBranchAddress("GenMHT", &GenMHT, &b_GenMHT);
  chain_->SetBranchAddress("GenMHT_Phi", &GenMHT_Phi, &b_GenMHT_Phi);
  chain_->SetBranchAddress("GenMinDeltaPhiEta24", &GenMinDeltaPhiEta24, &b_GenMinDeltaPhiEta24);
  chain_->SetBranchAddress("GenMinDeltaPhiEta5", &GenMinDeltaPhiEta5, &b_GenMinDeltaPhiEta5);
  chain_->SetBranchAddress("GenMinDeltaPhiIndexEta24", &GenMinDeltaPhiIndexEta24, &b_GenMinDeltaPhiIndexEta24);
  chain_->SetBranchAddress("GenMinDeltaPhiIndexEta5", &GenMinDeltaPhiIndexEta5, &b_GenMinDeltaPhiIndexEta5);
  chain_->SetBranchAddress("GenMu_GenMuFromTau", &GenMu_GenMuFromTau, &b_GenMu_GenMuFromTau);
  chain_->SetBranchAddress("GenMu_MT2Activity", &GenMu_MT2Activity, &b_GenMu_MT2Activity);
  chain_->SetBranchAddress("GenMu_RecoTrkAct", &GenMu_RecoTrkAct, &b_GenMu_RecoTrkAct);
  chain_->SetBranchAddress("GenMu_RecoTrkd3", &GenMu_RecoTrkd3, &b_GenMu_RecoTrkd3);
  chain_->SetBranchAddress("GenMu_RecoTrkIso", &GenMu_RecoTrkIso, &b_GenMu_RecoTrkIso);
  chain_->SetBranchAddress("GenMus", &GenMus, &b_GenMus);
  chain_->SetBranchAddress("genParticles", &genParticles, &b_genParticles);
  chain_->SetBranchAddress("genParticles_PDGid", &genParticles_PDGid, &b_genParticles_PDGid);
  chain_->SetBranchAddress("GenTau_GenTauHad", &GenTau_GenTauHad, &b_GenTau_GenTauHad);
  chain_->SetBranchAddress("GenTau_MT2Activity", &GenTau_MT2Activity, &b_GenTau_MT2Activity);
  chain_->SetBranchAddress("GenTauLeadRecoTrkAct", &GenTauLeadRecoTrkAct, &b_GenTauLeadRecoTrkAct);
  chain_->SetBranchAddress("GenTauLeadRecoTrkd3", &GenTauLeadRecoTrkd3, &b_GenTauLeadRecoTrkd3);
  chain_->SetBranchAddress("GenTauLeadRecoTrkIso", &GenTauLeadRecoTrkIso, &b_GenTauLeadRecoTrkIso);
  chain_->SetBranchAddress("GenTauLeadTrk", &GenTauLeadTrk, &b_GenTauLeadTrk);
  chain_->SetBranchAddress("GenTauNu", &GenTauNu, &b_GenTauNu);
  chain_->SetBranchAddress("GenTaus", &GenTaus, &b_GenTaus);
  chain_->SetBranchAddress("HBHEIsoNoiseFilter", &HBHEIsoNoiseFilter, &b_HBHEIsoNoiseFilter);
  chain_->SetBranchAddress("HBHENoiseFilter", &HBHENoiseFilter, &b_HBHENoiseFilter);
  chain_->SetBranchAddress("HT", &HT, &b_HT);
  chain_->SetBranchAddress("HTclean", &HTclean, &b_HTclean);
  chain_->SetBranchAddress("HTJetsMask", &HTJetsMask, &b_HTJetsMask);
  chain_->SetBranchAddress("isoElectronTracks", &isoElectronTracks, &b_isoElectronTracks);
  chain_->SetBranchAddress("IsolatedElectronTracksVeto", &IsolatedElectronTracksVeto, &b_IsolatedElectronTracksVeto);
  chain_->SetBranchAddress("IsolatedMuonTracksVeto", &IsolatedMuonTracksVeto, &b_IsolatedMuonTracksVeto);
  chain_->SetBranchAddress("IsolatedPionTracksVeto", &IsolatedPionTracksVeto, &b_IsolatedPionTracksVeto);
  chain_->SetBranchAddress("isoMuonTracks", &isoMuonTracks, &b_isoMuonTracks);
  chain_->SetBranchAddress("isoPionTracks", &isoPionTracks, &b_isoPionTracks);
  chain_->SetBranchAddress("JetID", &JetID, &b_JetID);
  chain_->SetBranchAddress("Jets", &Jets, &b_Jets);
  chain_->SetBranchAddress("Jets_bDiscriminatorCSV", &Jets_bDiscriminatorCSV, &b_Jets_bDiscriminatorCSV);
  chain_->SetBranchAddress("Jets_bDiscriminatorMVA", &Jets_bDiscriminatorMVA, &b_Jets_bDiscriminatorMVA);
  chain_->SetBranchAddress("Jets_chargedEmEnergyFraction", &Jets_chargedEmEnergyFraction, &b_Jets_chargedEmEnergyFraction);
  chain_->SetBranchAddress("Jets_chargedHadronEnergyFraction", &Jets_chargedHadronEnergyFraction, &b_Jets_chargedHadronEnergyFraction);
  chain_->SetBranchAddress("Jets_chargedHadronMultiplicity", &Jets_chargedHadronMultiplicity, &b_Jets_chargedHadronMultiplicity);
  chain_->SetBranchAddress("Jets_chargedMultiplicity", &Jets_chargedMultiplicity, &b_Jets_chargedMultiplicity);
  chain_->SetBranchAddress("Jets_electronMultiplicity", &Jets_electronMultiplicity, &b_Jets_electronMultiplicity);
  chain_->SetBranchAddress("Jets_hadronFlavor", &Jets_hadronFlavor, &b_Jets_hadronFlavor);
  chain_->SetBranchAddress("Jets_jetArea", &Jets_jetArea, &b_Jets_jetArea);
  chain_->SetBranchAddress("Jets_muonEnergyFraction", &Jets_muonEnergyFraction, &b_Jets_muonEnergyFraction);
  chain_->SetBranchAddress("Jets_muonMultiplicity", &Jets_muonMultiplicity, &b_Jets_muonMultiplicity);
  chain_->SetBranchAddress("Jets_neutralEmEnergyFraction", &Jets_neutralEmEnergyFraction, &b_Jets_neutralEmEnergyFraction);
  chain_->SetBranchAddress("Jets_neutralHadronEnergyFraction", &Jets_neutralHadronEnergyFraction, &b_Jets_neutralHadronEnergyFraction);
  chain_->SetBranchAddress("Jets_neutralHadronMultiplicity", &Jets_neutralHadronMultiplicity, &b_Jets_neutralHadronMultiplicity);
  chain_->SetBranchAddress("Jets_neutralMultiplicity", &Jets_neutralMultiplicity, &b_Jets_neutralMultiplicity);
  chain_->SetBranchAddress("Jets_partonFlavor", &Jets_partonFlavor, &b_Jets_partonFlavor);
  chain_->SetBranchAddress("Jets_photonEnergyFraction", &Jets_photonEnergyFraction, &b_Jets_photonEnergyFraction);
  chain_->SetBranchAddress("Jets_photonMultiplicity", &Jets_photonMultiplicity, &b_Jets_photonMultiplicity);
  chain_->SetBranchAddress("Leptons", &Leptons, &b_Leptons);
  chain_->SetBranchAddress("METFilters", &METFilters, &b_METFilters);
  chain_->SetBranchAddress("METPhi", &METPhi, &b_METPhi);
  chain_->SetBranchAddress("METPhiclean", &METPhiclean, &b_METPhiclean);
  chain_->SetBranchAddress("METPhiDown", &METPhiDown, &b_METPhiDown);
  chain_->SetBranchAddress("METPhiUp", &METPhiUp, &b_METPhiUp);
  chain_->SetBranchAddress("METPt", &METPt, &b_METPt);
  chain_->SetBranchAddress("METPtclean", &METPtclean, &b_METPtclean);
  chain_->SetBranchAddress("METPtDown", &METPtDown, &b_METPtDown);
  chain_->SetBranchAddress("METPtUp", &METPtUp, &b_METPtUp);
  chain_->SetBranchAddress("MHT", &MHT, &b_MHT);
  chain_->SetBranchAddress("MHT_Phi", &MHT_Phi, &b_MHT_Phi);
  chain_->SetBranchAddress("MHT_Phiclean", &MHT_Phiclean, &b_MHT_Phiclean);
  chain_->SetBranchAddress("MHTclean", &MHTclean, &b_MHTclean);
  chain_->SetBranchAddress("MHTJetsMask", &MHTJetsMask, &b_MHTJetsMask);
  chain_->SetBranchAddress("minDeltaPhiNames", &minDeltaPhiNames, &b_minDeltaPhiNames);
  chain_->SetBranchAddress("MuonCharge", &MuonCharge, &b_MuonCharge);
  chain_->SetBranchAddress("Muons", &Muons, &b_Muons);
  chain_->SetBranchAddress("nAllVertices", &nAllVertices, &b_nAllVertices);
  chain_->SetBranchAddress("NeutrinoLorentzVector", &NeutrinoLorentzVector, &b_NeutrinoLorentzVector);
  chain_->SetBranchAddress("NeutrinoMotherPdg", &NeutrinoMotherPdg, &b_NeutrinoMotherPdg);
  chain_->SetBranchAddress("NeutrinoPdg", &NeutrinoPdg, &b_NeutrinoPdg);
  chain_->SetBranchAddress("NJets", &NJets, &b_NJets);
  chain_->SetBranchAddress("NJetsclean", &NJetsclean, &b_NJetsclean);
  chain_->SetBranchAddress("nTAPElectronTracks", &nTAPElectronTracks, &b_nTAPElectronTracks);
  chain_->SetBranchAddress("nTAPMuonTracks", &nTAPMuonTracks, &b_nTAPMuonTracks);
  chain_->SetBranchAddress("nTAPPionTracks", &nTAPPionTracks, &b_nTAPPionTracks);
  chain_->SetBranchAddress("NumEvents", &NumEvents, &b_NumEvents);
  chain_->SetBranchAddress("NumInteractions", &NumInteractions, &b_NumInteractions);
  chain_->SetBranchAddress("NumPhotons", &NumPhotons, &b_NumPhotons);
  chain_->SetBranchAddress("NVtx", &NVtx, &b_NVtx);
  chain_->SetBranchAddress("PDFids", &PDFids, &b_PDFids);
  chain_->SetBranchAddress("PDFweights", &PDFweights, &b_PDFweights);
  chain_->SetBranchAddress("photon_genMatched", &photon_genMatched, &b_photon_genMatched);
  chain_->SetBranchAddress("photon_hadTowOverEM", &photon_hadTowOverEM, &b_photon_hadTowOverEM);
  chain_->SetBranchAddress("photon_hasPixelSeed", &photon_hasPixelSeed, &b_photon_hasPixelSeed);
  chain_->SetBranchAddress("photon_isEB", &photon_isEB, &b_photon_isEB);
  chain_->SetBranchAddress("photon_nonPrompt", &photon_nonPrompt, &b_photon_nonPrompt);
  chain_->SetBranchAddress("photon_passElectronVeto", &photon_passElectronVeto, &b_photon_passElectronVeto);
  chain_->SetBranchAddress("photon_pfChargedIso", &photon_pfChargedIso, &b_photon_pfChargedIso);
  chain_->SetBranchAddress("photon_pfChargedIsoRhoCorr", &photon_pfChargedIsoRhoCorr, &b_photon_pfChargedIsoRhoCorr);
  chain_->SetBranchAddress("photon_pfGammaIso", &photon_pfGammaIso, &b_photon_pfGammaIso);
  chain_->SetBranchAddress("photon_pfGammaIsoRhoCorr", &photon_pfGammaIsoRhoCorr, &b_photon_pfGammaIsoRhoCorr);
  chain_->SetBranchAddress("photon_pfNeutralIso", &photon_pfNeutralIso, &b_photon_pfNeutralIso);
  chain_->SetBranchAddress("photon_pfNeutralIsoRhoCorr", &photon_pfNeutralIsoRhoCorr, &b_photon_pfNeutralIsoRhoCorr);
  chain_->SetBranchAddress("photon_sigmaIetaIeta", &photon_sigmaIetaIeta, &b_photon_sigmaIetaIeta);
  chain_->SetBranchAddress("photonCands", &photonCands, &b_photonCands);
  chain_->SetBranchAddress("puSysDown", &puSysDown, &b_puSysDown);
  chain_->SetBranchAddress("puSysUp", &puSysUp, &b_puSysUp);
  chain_->SetBranchAddress("puWeight", &puWeight, &b_puWeight);
  chain_->SetBranchAddress("RJetDeltaPhi", &RJetDeltaPhi, &b_RJetDeltaPhi);
  chain_->SetBranchAddress("RJetMinDeltaPhiEta24", &RJetMinDeltaPhiEta24, &b_RJetMinDeltaPhiEta24);
  chain_->SetBranchAddress("RJetMinDeltaPhiEta5", &RJetMinDeltaPhiEta5, &b_RJetMinDeltaPhiEta5);
  chain_->SetBranchAddress("RJetMinDeltaPhiIndexEta24", &RJetMinDeltaPhiIndexEta24, &b_RJetMinDeltaPhiIndexEta24);
  chain_->SetBranchAddress("RJetMinDeltaPhiIndexEta5", &RJetMinDeltaPhiIndexEta5, &b_RJetMinDeltaPhiIndexEta5);
  chain_->SetBranchAddress("ScaleWeights", &ScaleWeights, &b_ScaleWeights);
  chain_->SetBranchAddress("selectedIDElectrons", &selectedIDElectrons, &b_selectedIDElectrons);
  chain_->SetBranchAddress("selectedIDElectrons_mediumID", &selectedIDElectrons_mediumID, &b_selectedIDElectrons_mediumID);
  chain_->SetBranchAddress("selectedIDElectrons_MiniIso", &selectedIDElectrons_MiniIso, &b_selectedIDElectrons_MiniIso);
  chain_->SetBranchAddress("selectedIDElectrons_MT2Activity", &selectedIDElectrons_MT2Activity, &b_selectedIDElectrons_MT2Activity);
  chain_->SetBranchAddress("selectedIDElectrons_MTW", &selectedIDElectrons_MTW, &b_selectedIDElectrons_MTW);
  chain_->SetBranchAddress("selectedIDIsoElectrons_mediumID", &selectedIDIsoElectrons_mediumID, &b_selectedIDIsoElectrons_mediumID);
  chain_->SetBranchAddress("selectedIDIsoElectrons_MT2Activity", &selectedIDIsoElectrons_MT2Activity, &b_selectedIDIsoElectrons_MT2Activity);
  chain_->SetBranchAddress("selectedIDIsoElectrons_MTW", &selectedIDIsoElectrons_MTW, &b_selectedIDIsoElectrons_MTW);
  chain_->SetBranchAddress("selectedIDIsoElectrons_PTW", &selectedIDIsoElectrons_PTW, &b_selectedIDIsoElectrons_PTW);
  chain_->SetBranchAddress("selectedIDIsoMuons_MT2Activity", &selectedIDIsoMuons_MT2Activity, &b_selectedIDIsoMuons_MT2Activity);
  chain_->SetBranchAddress("selectedIDIsoMuons_MTW", &selectedIDIsoMuons_MTW, &b_selectedIDIsoMuons_MTW);
  chain_->SetBranchAddress("selectedIDIsoMuons_PTW", &selectedIDIsoMuons_PTW, &b_selectedIDIsoMuons_PTW);
  chain_->SetBranchAddress("selectedIDMuons", &selectedIDMuons, &b_selectedIDMuons);
  chain_->SetBranchAddress("selectedIDMuons_MiniIso", &selectedIDMuons_MiniIso, &b_selectedIDMuons_MiniIso);
  chain_->SetBranchAddress("selectedIDMuons_MT2Activity", &selectedIDMuons_MT2Activity, &b_selectedIDMuons_MT2Activity);
  chain_->SetBranchAddress("selectedIDMuons_MTW", &selectedIDMuons_MTW, &b_selectedIDMuons_MTW);
  chain_->SetBranchAddress("slimJet", &slimJet, &b_slimJet);
  chain_->SetBranchAddress("slimJet_slimJetID", &slimJet_slimJetID, &b_slimJet_slimJetID);
  chain_->SetBranchAddress("slimJetJECdown", &slimJetJECdown, &b_slimJetJECdown);
  chain_->SetBranchAddress("slimJetJECdown_slimJetID", &slimJetJECdown_slimJetID, &b_slimJetJECdown_slimJetID);
  chain_->SetBranchAddress("slimJetJECup", &slimJetJECup, &b_slimJetJECup);
  chain_->SetBranchAddress("slimJetJECup_slimJetID", &slimJetJECup_slimJetID, &b_slimJetJECup_slimJetID);
  chain_->SetBranchAddress("SusyLSPMass", &SusyLSPMass, &b_SusyLSPMass);
  chain_->SetBranchAddress("SusyMotherMass", &SusyMotherMass, &b_SusyMotherMass);
  chain_->SetBranchAddress("TagLeptonHighPT", &TagLeptonHighPT, &b_TagLeptonHighPT);
  chain_->SetBranchAddress("TAPElectronTracks", &TAPElectronTracks, &b_TAPElectronTracks);
  chain_->SetBranchAddress("TAPElectronTracks_activity", &TAPElectronTracks_activity, &b_TAPElectronTracks_activity);
  chain_->SetBranchAddress("TAPElectronTracks_chg", &TAPElectronTracks_chg, &b_TAPElectronTracks_chg);
  chain_->SetBranchAddress("TAPElectronTracks_mT", &TAPElectronTracks_mT, &b_TAPElectronTracks_mT);
  chain_->SetBranchAddress("TAPElectronTracks_trkiso", &TAPElectronTracks_trkiso, &b_TAPElectronTracks_trkiso);
  chain_->SetBranchAddress("TAPMuonTracks", &TAPMuonTracks, &b_TAPMuonTracks);
  chain_->SetBranchAddress("TAPMuonTracks_activity", &TAPMuonTracks_activity, &b_TAPMuonTracks_activity);
  chain_->SetBranchAddress("TAPMuonTracks_chg", &TAPMuonTracks_chg, &b_TAPMuonTracks_chg);
  chain_->SetBranchAddress("TAPMuonTracks_mT", &TAPMuonTracks_mT, &b_TAPMuonTracks_mT);
  chain_->SetBranchAddress("TAPMuonTracks_trkiso", &TAPMuonTracks_trkiso, &b_TAPMuonTracks_trkiso);
  chain_->SetBranchAddress("TAPPionTracks", &TAPPionTracks, &b_TAPPionTracks);
  chain_->SetBranchAddress("TAPPionTracks_activity", &TAPPionTracks_activity, &b_TAPPionTracks_activity);
  chain_->SetBranchAddress("TAPPionTracks_chg", &TAPPionTracks_chg, &b_TAPPionTracks_chg);
  chain_->SetBranchAddress("TAPPionTracks_mT", &TAPPionTracks_mT, &b_TAPPionTracks_mT);
  chain_->SetBranchAddress("TAPPionTracks_trkiso", &TAPPionTracks_trkiso, &b_TAPPionTracks_trkiso);
  chain_->SetBranchAddress("TauDecayCands", &TauDecayCands, &b_TauDecayCands);
  chain_->SetBranchAddress("TauDecayCands_momInd", &TauDecayCands_momInd, &b_TauDecayCands_momInd);
  chain_->SetBranchAddress("TauDecayCands_pdgID", &TauDecayCands_pdgID, &b_TauDecayCands_pdgID);
  chain_->SetBranchAddress("TriggerNames", &TriggerNames, &b_TriggerNames);
  chain_->SetBranchAddress("TriggerPass", &TriggerPass, &b_TriggerPass);
  chain_->SetBranchAddress("TriggerPrescales", &TriggerPrescales, &b_TriggerPrescales);
  chain_->SetBranchAddress("TrueNumInteractions", &TrueNumInteractions, &b_TrueNumInteractions);
  chain_->SetBranchAddress("Weight", &Weight, &b_Weight);
  chain_->SetBranchAddress("ZNum", &ZNum, &b_ZNum);
  chain_->SetBranchAddress("Zp4", &Zp4, &b_Zp4);

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
