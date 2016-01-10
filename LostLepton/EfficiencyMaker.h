//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Jan  8 13:05:22 2016 by ROOT version 6.02/05
// from TTree mc_exp_tree/tree for calculating lepton efficiencies and expected number of background events from MC
// found on file: exp_test.root
//////////////////////////////////////////////////////////

#ifndef EfficiencyMaker_h
#define EfficiencyMaker_h

#include <THEff.h>


#include <iostream>
#include <vector>
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TH2D.h>
#include <TH1D.h>
#include "TVector2.h"
#include <cmath>
#include "TCanvas.h"
#include "TEfficiency.h"
#include "TGraphAsymmErrors.h"
#include "TLorentzVector.h"
#include "TSystem.h"
#include <TLegend.h>



////////////////////////
//////// Usually no changes necessary
///////////////////////

using std::vector;
using std::cout;
using std::cerr;
using std::endl;

class EfficiencyMaker {
public :

  // efficiency histograms
  
  TH2Eff *MuAccHTMHT_NJets78_;
  TH2Eff *MuAccHTMHT_NJets9Inf_;
  TH2Eff *MuRecoActivityPT_;
  TH2Eff *MuIsoActivityPT_;
  TH2Eff *MuMTWHTNJets_;
  TH1Eff* MuDiLepContributionMTWNJets_;
  TH1Eff* MuDiLepMTWNJets_;

  
  TH2Eff *ElecAccHTMHT_NJets78_;
  TH2Eff *ElecAccHTMHT_NJets9Inf_;
  TH2Eff *ElecRecoActivityPT_;
  TH2Eff *ElecIsoActivityPT_;
  //  TH2Eff *ElecMTWHTNJets_;

  TH2Eff *MuIsoTrackReductionHTMHT_NJetsHigh_;
  TH2Eff *ElecIsoTrackReductionHTMHT_NJetsHigh_;
  TH2Eff *PionIsoTrackReductionHTMHT_NJetsHigh_;

   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   UInt_t          EvtNum;
   Double_t        HT;
   Double_t        MHT;
   Int_t           NJets;
   Int_t           BTags;
   Int_t           isoTracks;
   UShort_t        Bin;
   Double_t        genHT;
   Int_t           NVtx;
   Double_t        DeltaPhi1;
   Double_t        DeltaPhi2;
   Double_t        DeltaPhi3;
   Double_t        DeltaPhi4;
   Double_t        Weight;
   Double_t        MET;
   Double_t        METPhi;
   UShort_t        GenMuNum;
   vector<TLorentzVector> *GenMus;
   vector<double>  *GenMu_MT2Activity;
   UShort_t        GenElecNum;
   vector<TLorentzVector> *GenEls;
   vector<double>  *GenElec_MT2Activity;
   UShort_t        Expectation;
   UShort_t        ExpectationReductionIsoTrack;
   UShort_t        muAcc;
   UShort_t        muReco;
   UShort_t        muIso;
   Float_t         MTW;
   UShort_t        elecAcc;
   UShort_t        elecReco;
   UShort_t        elecIso;
   UShort_t        selectedIDIsoMuonsNum;
   vector<TLorentzVector> *selectedIDIsoMuons;
   vector<unsigned short> *muIsoMatched;
   vector<double>  *selectedIDIsoMuons_MTW;
   vector<double>  *selectedIDIsoMuons_MT2Activity;
   UShort_t        selectedIDMuonsNum;
   vector<TLorentzVector> *selectedIDMuons;
   vector<unsigned short> *selectedIDMuonsMatched;
   vector<double>  *selectedIDMuons_MTW;
   vector<double>  *selectedIDMuons_MiniIso;
   UShort_t        selectedIDIsoElectronsNum;
   vector<TLorentzVector> *selectedIDIsoElectrons;
   vector<unsigned short> *elecIsoMatched;
   vector<double>  *selectedIDIsoElectrons_MTW;
   vector<double>  *selectedIDIsoElectrons_MT2Activity;
   UShort_t        selectedIDElectronsNum;
   vector<TLorentzVector> *selectedIDElectrons;
   vector<unsigned short> *selectedIDElectronsMatched;
   vector<double>  *selectedIDElectrons_MTW;
   vector<double>  *selectedIDElectrons_MiniIso;
   Int_t           isoElectronTracks;
   Int_t           isoMuonTracks;
   Int_t           isoPionTracks;
   UShort_t        ExpectationDiLep;
   UShort_t        MuDiLepControlSample;
   UShort_t        ElecDiLepControlSample;

   // List of branches
   TBranch        *b_EvtNum;   //!
   TBranch        *b_HT;   //!
   TBranch        *b_MHT;   //!
   TBranch        *b_NJets;   //!
   TBranch        *b_BTags;   //!
   TBranch        *b_isoTracks;   //!
   TBranch        *b_Bin;   //!
   TBranch        *b_genHT;   //!
   TBranch        *b_NVtx;   //!
   TBranch        *b_DeltaPhi1;   //!
   TBranch        *b_DeltaPhi2;   //!
   TBranch        *b_DeltaPhi3;   //!
   TBranch        *b_DeltaPhi4;   //!
   TBranch        *b_Weight;   //!
   TBranch        *b_MET;   //!
   TBranch        *b_METPhi;   //!
   TBranch        *b_GenMuNum;   //!
   TBranch        *b_GenMus;   //!
   TBranch        *b_GenMu_MT2Activity;   //!
   TBranch        *b_GenElecNum;   //!
   TBranch        *b_GenEls;   //!
   TBranch        *b_GenElec_MT2Activity;   //!
   TBranch        *b_Expectation;   //!
   TBranch        *b_ExpectationReductionIsoTrack;   //!
   TBranch        *b_muAcc;   //!
   TBranch        *b_muReco;   //!
   TBranch        *b_muIso;   //!
   TBranch        *b_MTW;   //!
   TBranch        *b_elecAcc;   //!
   TBranch        *b_elecReco;   //!
   TBranch        *b_elecIso;   //!
   TBranch        *b_selectedIDIsoMuonsNum;   //!
   TBranch        *b_selectedIDIsoMuons;   //!
   TBranch        *b_muIsoMatched;   //!
   TBranch        *b_selectedIDIsoMuons_MTW;   //!
   TBranch        *b_selectedIDIsoMuons_MT2Activity;   //!
   TBranch        *b_selectedIDMuonsNum;   //!
   TBranch        *b_selectedIDMuons;   //!
   TBranch        *b_selectedIDMuonsMatched;   //!
   TBranch        *b_selectedIDMuons_MTW;   //!
   TBranch        *b_selectedIDMuons_MiniIso;   //!
   TBranch        *b_selectedIDIsoElectronsNum;   //!
   TBranch        *b_selectedIDIsoElectrons;   //!
   TBranch        *b_elecIsoMatched;   //!
   TBranch        *b_selectedIDIsoElectrons_MTW;   //!
   TBranch        *b_selectedIDIsoElectrons_MT2Activity;   //!
   TBranch        *b_selectedIDElectronsNum;   //!
   TBranch        *b_selectedIDElectrons;   //!
   TBranch        *b_selectedIDElectronsMatched;   //!
   TBranch        *b_selectedIDElectrons_MTW;   //!
   TBranch        *b_selectedIDElectrons_MiniIso;   //!
   TBranch        *b_isoElectronTracks;   //!
   TBranch        *b_isoMuonTracks;   //!
   TBranch        *b_isoPionTracks;   //!
   TBranch        *b_ExpectationDiLep;   //!
   TBranch        *b_MuDiLepControlSample;   //!
   TBranch        *b_ElecDiLepControlSample;   //!

   EfficiencyMaker(TTree *tree=0);
   virtual ~EfficiencyMaker();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Run(std::string outputFileName);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef EfficiencyMaker_cxx
EfficiencyMaker::EfficiencyMaker(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("exp_test.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("exp_test.root");
      }
      f->GetObject("mc_exp_tree",tree);

   }
   Init(tree);
}

EfficiencyMaker::~EfficiencyMaker()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t EfficiencyMaker::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t EfficiencyMaker::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void EfficiencyMaker::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   GenMus = 0;
   GenMu_MT2Activity = 0;
   GenEls = 0;
   GenElec_MT2Activity = 0;
   selectedIDIsoMuons = 0;
   muIsoMatched = 0;
   selectedIDIsoMuons_MTW = 0;
   selectedIDIsoMuons_MT2Activity = 0;
   selectedIDMuons = 0;
   selectedIDMuonsMatched = 0;
   selectedIDMuons_MTW = 0;
   selectedIDMuons_MiniIso = 0;
   selectedIDIsoElectrons = 0;
   elecIsoMatched = 0;
   selectedIDIsoElectrons_MTW = 0;
   selectedIDIsoElectrons_MT2Activity = 0;
   selectedIDElectrons = 0;
   selectedIDElectronsMatched = 0;
   selectedIDElectrons_MTW = 0;
   selectedIDElectrons_MiniIso = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("EvtNum", &EvtNum, &b_EvtNum);
   fChain->SetBranchAddress("HT", &HT, &b_HT);
   fChain->SetBranchAddress("MHT", &MHT, &b_MHT);
   fChain->SetBranchAddress("NJets", &NJets, &b_NJets);
   fChain->SetBranchAddress("BTags", &BTags, &b_BTags);
   fChain->SetBranchAddress("isoTracks", &isoTracks, &b_isoTracks);
   fChain->SetBranchAddress("Bin", &Bin, &b_Bin);
   fChain->SetBranchAddress("genHT", &genHT, &b_genHT);
   fChain->SetBranchAddress("NVtx", &NVtx, &b_NVtx);
   fChain->SetBranchAddress("DeltaPhi1", &DeltaPhi1, &b_DeltaPhi1);
   fChain->SetBranchAddress("DeltaPhi2", &DeltaPhi2, &b_DeltaPhi2);
   fChain->SetBranchAddress("DeltaPhi3", &DeltaPhi3, &b_DeltaPhi3);
   fChain->SetBranchAddress("DeltaPhi4", &DeltaPhi4, &b_DeltaPhi4);
   fChain->SetBranchAddress("Weight", &Weight, &b_Weight);
   fChain->SetBranchAddress("MET", &MET, &b_MET);
   fChain->SetBranchAddress("METPhi", &METPhi, &b_METPhi);
   fChain->SetBranchAddress("GenMuNum", &GenMuNum, &b_GenMuNum);
   fChain->SetBranchAddress("GenMus", &GenMus, &b_GenMus);
   fChain->SetBranchAddress("GenMu_MT2Activity", &GenMu_MT2Activity, &b_GenMu_MT2Activity);
   fChain->SetBranchAddress("GenElecNum", &GenElecNum, &b_GenElecNum);
   fChain->SetBranchAddress("GenEls", &GenEls, &b_GenEls);
   fChain->SetBranchAddress("GenElec_MT2Activity", &GenElec_MT2Activity, &b_GenElec_MT2Activity);
   fChain->SetBranchAddress("Expectation", &Expectation, &b_Expectation);
   fChain->SetBranchAddress("ExpectationReductionIsoTrack", &ExpectationReductionIsoTrack, &b_ExpectationReductionIsoTrack);
   fChain->SetBranchAddress("muAcc", &muAcc, &b_muAcc);
   fChain->SetBranchAddress("muReco", &muReco, &b_muReco);
   fChain->SetBranchAddress("muIso", &muIso, &b_muIso);
   fChain->SetBranchAddress("MTW", &MTW, &b_MTW);
   fChain->SetBranchAddress("elecAcc", &elecAcc, &b_elecAcc);
   fChain->SetBranchAddress("elecReco", &elecReco, &b_elecReco);
   fChain->SetBranchAddress("elecIso", &elecIso, &b_elecIso);
   fChain->SetBranchAddress("selectedIDIsoMuonsNum", &selectedIDIsoMuonsNum, &b_selectedIDIsoMuonsNum);
   fChain->SetBranchAddress("selectedIDIsoMuons", &selectedIDIsoMuons, &b_selectedIDIsoMuons);
   fChain->SetBranchAddress("muIsoMatched", &muIsoMatched, &b_muIsoMatched);
   fChain->SetBranchAddress("selectedIDIsoMuons_MTW", &selectedIDIsoMuons_MTW, &b_selectedIDIsoMuons_MTW);
   fChain->SetBranchAddress("selectedIDIsoMuons_MT2Activity", &selectedIDIsoMuons_MT2Activity, &b_selectedIDIsoMuons_MT2Activity);
   fChain->SetBranchAddress("selectedIDMuonsNum", &selectedIDMuonsNum, &b_selectedIDMuonsNum);
   fChain->SetBranchAddress("selectedIDMuons", &selectedIDMuons, &b_selectedIDMuons);
   fChain->SetBranchAddress("selectedIDMuonsMatched", &selectedIDMuonsMatched, &b_selectedIDMuonsMatched);
   fChain->SetBranchAddress("selectedIDMuons_MTW", &selectedIDMuons_MTW, &b_selectedIDMuons_MTW);
   fChain->SetBranchAddress("selectedIDMuons_MiniIso", &selectedIDMuons_MiniIso, &b_selectedIDMuons_MiniIso);
   fChain->SetBranchAddress("selectedIDIsoElectronsNum", &selectedIDIsoElectronsNum, &b_selectedIDIsoElectronsNum);
   fChain->SetBranchAddress("selectedIDIsoElectrons", &selectedIDIsoElectrons, &b_selectedIDIsoElectrons);
   fChain->SetBranchAddress("elecIsoMatched", &elecIsoMatched, &b_elecIsoMatched);
   fChain->SetBranchAddress("selectedIDIsoElectrons_MTW", &selectedIDIsoElectrons_MTW, &b_selectedIDIsoElectrons_MTW);
   fChain->SetBranchAddress("selectedIDIsoElectrons_MT2Activity", &selectedIDIsoElectrons_MT2Activity, &b_selectedIDIsoElectrons_MT2Activity);
   fChain->SetBranchAddress("selectedIDElectronsNum", &selectedIDElectronsNum, &b_selectedIDElectronsNum);
   fChain->SetBranchAddress("selectedIDElectrons", &selectedIDElectrons, &b_selectedIDElectrons);
   fChain->SetBranchAddress("selectedIDElectronsMatched", &selectedIDElectronsMatched, &b_selectedIDElectronsMatched);
   fChain->SetBranchAddress("selectedIDElectrons_MTW", &selectedIDElectrons_MTW, &b_selectedIDElectrons_MTW);
   fChain->SetBranchAddress("selectedIDElectrons_MiniIso", &selectedIDElectrons_MiniIso, &b_selectedIDElectrons_MiniIso);
   fChain->SetBranchAddress("isoElectronTracks", &isoElectronTracks, &b_isoElectronTracks);
   fChain->SetBranchAddress("isoMuonTracks", &isoMuonTracks, &b_isoMuonTracks);
   fChain->SetBranchAddress("isoPionTracks", &isoPionTracks, &b_isoPionTracks);
   fChain->SetBranchAddress("ExpectationDiLep", &ExpectationDiLep, &b_ExpectationDiLep);
   fChain->SetBranchAddress("MuDiLepControlSample", &MuDiLepControlSample, &b_MuDiLepControlSample);
   fChain->SetBranchAddress("ElecDiLepControlSample", &ElecDiLepControlSample, &b_ElecDiLepControlSample);
   Notify();
}

Bool_t EfficiencyMaker::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void EfficiencyMaker::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t EfficiencyMaker::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef EfficiencyMaker_cxx
