// Step 4: Compare the expected and predicted number of events from MC to calculate a non-closure systematic, then run the full prediction on data
#include <TChain.h>
#include <TROOT.h>
#include <TFile.h>
#include <TSelector.h>
#include <TTree.h>
#include <TH2F.h>
#include <TH1D.h>
#include <TProfile.h>
#include "TVector2.h"
#include <cmath>
#include "TCanvas.h"
#include "TEfficiency.h"
#include "TGraphAsymmErrors.h"
#include <iostream>
#include <vector>
#include "LLTools.h"
#include "THEff.h"

void SaveClosure(TH1D* prediction, TH1D* expectation, TDirectory* Folder);
void SetBinLabel(TH1D* hist);
void SaveFraction(TH1D* Top, TH1D* Bottom, TDirectory* dir);
void addUncertainties(TH1D* total, std::vector<TH1D*> uncertainties, bool upperUnc);


void RunClosureAndFullEstimation()
{

  // General Settings

  // First we'll compare the expected number of background events from MC
  // with the number predicted by running the full method on the MC
  // This is the closure test--we'll use it to assign a systematic uncertainty on the data prediction
  TString InputPath_Expectation("Expectation.root");
  TString InputPath_Prediction("Prediction.root");
  // here's where we'll put the data
  TString InputPath_Prediction_Data("Prediction_data.root"); // Use same path as above if pure MC prediction wanted
  TString OutputPath_Closure("Closure.root");
  TString OutputPath_Prediction("LLPrediction.root");

  // If you want to compare MC to MC set this to true. E.g. prediction with and without signal contamination
  bool useMCForDataTree = false;

  // Scale all MC weights by this factor
  Double_t scaleFactorWeight = 2262;  //in units of [pb] //<-check------------------------

  // Do approximation of statistical uncertainties if full MC statistics are used (stat. unc. then refers to a given luminosity of data)
  // Leave at 'false' if doing a closure test so stat. uncertainty is the one using full MC statistics
  bool approxStatUncertainty = false;


  int nSearchBinsTotal = 6;
  UShort_t         SearchBin=0;


  //Declaration of leaves types for both trees (Expectation/Prediction)
  UShort_t         Bin;
  Double_t         Weight;
  Double_t         scaledWeight;
  Double_t         HT;
  Double_t         MHT;
  Int_t            NJets;
  Int_t            BTags;

  UShort_t        Expectation;
  UShort_t        muAcc;
  UShort_t        muReco;
  UShort_t        muIso;
  UShort_t        elecAcc;
  UShort_t        elecReco;
  UShort_t        elecIso;
  Int_t           isoTracks;
  UShort_t        ExpectationDiLep;
  UShort_t        ExpectationReductionIsoTrack;

  Float_t         MTW;
  UShort_t        selectedIDIsoMuonsNum;
  UShort_t        selectedIDIsoElectronsNum;
  
  Float_t         totalWeightDiLep;
  Float_t         totalWeightDiLepIsoTrackReducedCombined;
  
  Float_t         muIsoWeight;
  Float_t         muRecoWeight;
  Float_t         muAccWeight;

  Float_t         elecAccWeight;
  Float_t         elecRecoWeight;
  Float_t         elecIsoWeight;

  Float_t         totalStatUp;
  Float_t         totalStatDown;
  // Float_t         totalSysUp;
  // Float_t         totalSysDown;
  Float_t         nonClosureUp;
  Float_t         nonClosureDown;
  // Float_t         diBosonUp;
  // Float_t         diBosonDown;
  Float_t         totalUncUp;
  Float_t         totalUncDown;

  

  //closure test
  TFile *outPutFile = new TFile(OutputPath_Closure,"RECREATE"); 
  outPutFile->cd();

  double totalExp=0, totalPre=0;
  double totalExpError=0, totalPreError=0;
  double totalPreMu=0, totalPreMuError=0;
  double totalExpIsoTrack=0, totalExpIsoTrackError=0;

  double totalPreIsoTrack=0, totalPreIsoTrackError=0;
  
  double totalExpMuAcc=0, totalExpMuReco=0, totalExpMuIso=0;
  double totalPreMuAcc=0, totalPreMuReco=0, totalPreMuIso=0;
  double totalExpElecAcc=0, totalExpElecReco=0, totalExpElecIso=0;
  double totalPreElecAcc=0, totalPreElecReco=0, totalPreElecIso=0;
	
  TH1D* ControlSampleMu_ = new TH1D("ControlSampleMu", "ControlSampleMu", nSearchBinsTotal, 0.5, nSearchBinsTotal+0.5);
	
  TH1D* totalExpectation_ = new TH1D("TotalLostLeptonExpecation", "TotalLostLeptonExpecation", nSearchBinsTotal, 0.5, nSearchBinsTotal+0.5);
  TH1D* totalPrediction_ = new TH1D("TotalLostLeptonPrediction", "TotalLostLeptonPrediction", nSearchBinsTotal, 0.5, nSearchBinsTotal+0.5);
	
  TH1D* totalExpectationIsoTrackReduction_ = new TH1D("TotalLostLeptonExpecationIsoTrackReduction", "TotalLostLeptonExpecationIsoTrackReduction", nSearchBinsTotal, 0.5, nSearchBinsTotal+0.5);
  TH1D* totalPredictionIsoTrackReduction_ = new TH1D("TotalLostLeptonPredictionIsoTrackReduction", "TotalLostLeptonPredictionIsoTrackReduction", nSearchBinsTotal, 0.5, nSearchBinsTotal+0.5);
	
  // separted closure
  TH1D* totalExpectationMuAcc_ = new TH1D("TotalLostLeptonExpecationMuAcc", "TotalLostLeptonExpecationMuAcc", nSearchBinsTotal, 0.5, nSearchBinsTotal+0.5);
  TH1D* totalPredictionMuCSMuAcc_ = new TH1D("TotalLostLeptonPredictionMuCSMuAcc", "TotalLostLeptonPredictionMuCSMuAcc", nSearchBinsTotal, 0.5, nSearchBinsTotal+0.5);
	
  TH1D* totalExpectationMuReco_ = new TH1D("TotalLostLeptonExpecationMuReco", "TotalLostLeptonExpecationMuReco", nSearchBinsTotal, 0.5, nSearchBinsTotal+0.5);
  TH1D* totalPredictionMuCSMuReco_ = new TH1D("TotalLostLeptonPredictionMuCSMuReco", "TotalLostLeptonPredictionMuCSMuReco", nSearchBinsTotal, 0.5, nSearchBinsTotal+0.5);
	
  TH1D* totalExpectationMuIso_ = new TH1D("TotalLostLeptonExpecationMuIso", "TotalLostLeptonExpecationMuIso", nSearchBinsTotal, 0.5, nSearchBinsTotal+0.5);
  TH1D* totalPredictionMuCSMuIso_ = new TH1D("TotalLostLeptonPredictionMuCSMuIso", "TotalLostLeptonPredictionMuCSMuIso", nSearchBinsTotal, 0.5, nSearchBinsTotal+0.5);
	
  TH1D* totalExpectationElecAcc_ = new TH1D("TotalLostLeptonExpecationElecAcc", "TotalLostLeptonExpecationElecAcc", nSearchBinsTotal, 0.5, nSearchBinsTotal+0.5);
  TH1D* totalPredictionMuCSElecAcc_ = new TH1D("TotalLostLeptonPredictionMuCSElecAcc", "TotalLostLeptonPredictionMuCSElecAcc", nSearchBinsTotal, 0.5, nSearchBinsTotal+0.5);
	
  TH1D* totalExpectationElecReco_ = new TH1D("TotalLostLeptonExpecationElecReco", "TotalLostLeptonExpecationElecReco", nSearchBinsTotal, 0.5, nSearchBinsTotal+0.5);
  TH1D* totalPredictionMuCSElecReco_ = new TH1D("TotalLostLeptonPredictionMuCSElecReco", "TotalLostLeptonPredictionMuCSElecReco", nSearchBinsTotal, 0.5, nSearchBinsTotal+0.5);
	
  TH1D* totalExpectationElecIso_ = new TH1D("TotalLostLeptonExpecationElecIso", "TotalLostLeptonExpecationElecIso", nSearchBinsTotal, 0.5, nSearchBinsTotal+0.5);
  TH1D* totalPredictionMuCSElecIso_ = new TH1D("TotalLostLeptonPredictionMuCSElecIso", "TotalLostLeptonPredictionMuCSElecIso", nSearchBinsTotal, 0.5, nSearchBinsTotal+0.5);


  // Define histrograms to do totalPrediction per SearchBin
  TH1D* totalExp_LL_ = new TH1D("totalExp_LL","totalExp_LL", nSearchBinsTotal, 0.5, nSearchBinsTotal+0.5);
  TH1D* nEvtsExp_LL_ = new TH1D("nEvtsExp_LL","nEvtsExp_LL", nSearchBinsTotal, 0.5, nSearchBinsTotal+0.5);

  TH1D* totalExp_HT_LL_ = new TH1D("totalExp_HT_LL","totalExp_HT_LL", 20, 500., 2500.);
  TH1D* totalExp_MHT_LL_ = new TH1D("totalExp_MHT_LL","totalExp_MHT_LL", 20, 200., 1200.);
  TH1D* totalExp_NJets_LL_ = new TH1D("totalExp_NJets_LL","totalExp_NJets_LL", 7, 3.5, 10.5);
  TH1D* totalExp_BTags_LL_ = new TH1D("totalExp_BTags_LL","totalExp_BTags_LL", 7, -0.5, 6.5);

  TH1D* totalPred_LL_ = new TH1D("totalPred_LL","totalPred_LL", nSearchBinsTotal, 0.5, nSearchBinsTotal+0.5);
  TH1D* totalPred_HT_LL_ = new TH1D("totalPred_HT_LL","totalPred_HT_LL", 20, 500., 2500.);
  TH1D* totalPred_MHT_LL_ = new TH1D("totalPred_MHT_LL","totalPred_MHT_LL", 20, 200., 1200.);
  TH1D* totalPred_NJets_LL_ = new TH1D("totalPred_NJets_LL","totalPred_NJets_LL", 7, 3.5, 10.5);
  TH1D* totalPred_BTags_LL_ = new TH1D("totalPred_BTags_LL","totalPred_BTags_LL", 7, -0.5, 6.5);

  TH1D* totalPredNonClosureUp_LL_ = new TH1D("totalPredNonClosureUp_LL","totalPredNonClosureUp_LL", nSearchBinsTotal, 0.5, nSearchBinsTotal+0.5);
  TH1D* totalPredNonClosureDown_LL_ = new TH1D("totalPredNonClosureDown_LL","totalPredNonClosureDown_LL", nSearchBinsTotal, 0.5, nSearchBinsTotal+0.5);

  TH1D* totalCS_LL_ = new TH1D("totalCS_LL","totalCS_LL", nSearchBinsTotal, 0.5, nSearchBinsTotal+0.5);
  TH1D* nEvtsCS_LL_ = new TH1D("nEvtsCS_LL","nEvtsCS_LL", nSearchBinsTotal, 0.5, nSearchBinsTotal+0.5);

  TProfile* avgWeight_LL_ = new TProfile("avgWeight_LL","avgWeight_LL", nSearchBinsTotal, 0.5, nSearchBinsTotal+0.5);

  totalPred_LL_->Sumw2();
  totalPredNonClosureUp_LL_->Sumw2();
  totalPredNonClosureDown_LL_->Sumw2();

  totalCS_LL_->Sumw2();

  avgWeight_LL_->Sumw2();


  // Define histrograms to do totalPrediction per SearchBin (MC)
  TH1D* totalPred_LL_MC_ = new TH1D("totalPred_LL_MC","totalPred_LL_MC", nSearchBinsTotal, 0.5, nSearchBinsTotal+0.5);
  TH1D* totalPred_HT_LL_MC_ = new TH1D("totalPred_HT_LL_MC","totalPred_HT_LL_MC", 20, 500., 2500.);
  TH1D* totalPred_MHT_LL_MC_ = new TH1D("totalPred_MHT_LL_MC","totalPred_MHT_LL_MC", 20, 200., 1200.);
  TH1D* totalPred_NJets_LL_MC_ = new TH1D("totalPred_NJets_LL_MC","totalPred_NJets_LL_MC", 7, 3.5, 10.5);
  TH1D* totalPred_BTags_LL_MC_ = new TH1D("totalPred_BTags_LL_MC","totalPred_BTags_LL_MC", 7, -0.5, 6.5);


  TH1D* totalPredNonClosureUp_LL_MC_ = new TH1D("totalPredNonClosureUp_LL_MC","totalPredNonClosureUp_LL_MC", nSearchBinsTotal, 0.5, nSearchBinsTotal+0.5);
  TH1D* totalPredNonClosureDown_LL_MC_ = new TH1D("totalPredNonClosureDown_LL_MC","totalPredNonClosureDown_LL_MC", nSearchBinsTotal, 0.5, nSearchBinsTotal+0.5);

  TH1D* totalCS_LL_MC_ = new TH1D("totalCS_LL_MC","totalCS_LL_MC", nSearchBinsTotal, 0.5, nSearchBinsTotal+0.5);
  TH1D* nEvtsCS_LL_MC_ = new TH1D("nEvtsCS_LL_MC","nEvtsCS_LL_MC", nSearchBinsTotal, 0.5, nSearchBinsTotal+0.5);

  TProfile* avgWeight_LL_MC_ = new TProfile("avgWeight_LL_MC","avgWeight_LL_MC", nSearchBinsTotal, 0.5, nSearchBinsTotal+0.5);
  TProfile* avgWeight_HT_LL_MC_ = new TProfile("avgWeight_HT_LL_MC","avgWeight_HT_LL_MC", 20, 500., 2500.);
  TProfile* avgWeight_MHT_LL_MC_ = new TProfile("avgWeight_MHT_LL_MC","avgWeight_MHT_LL_MC", 20, 200., 1200.);
  TProfile* avgWeight_NJets_LL_MC_ = new TProfile("avgWeight_NJets_LL_MC","avgWeight_NJets_LL_MC", 7, 3.5, 10.5);
  TProfile* avgWeight_BTags_LL_MC_ = new TProfile("avgWeight_BTags_LL_MC","avgWeight_BTags_LL_MC", 7, -0.5, 6.5);


  totalPred_LL_MC_->Sumw2();
  totalPredNonClosureUp_LL_MC_->Sumw2();
  totalPredNonClosureDown_LL_MC_->Sumw2();

  totalCS_LL_MC_->Sumw2();

  avgWeight_LL_MC_->Sumw2();



	
  //Expectation Tree
  gROOT->Reset();
  TFile *fExp = (TFile*)gROOT->GetListOfFiles()->FindObject(InputPath_Expectation);
  if (!fExp) {
    fExp = new TFile(InputPath_Expectation);
  }
  
  TTree* LostLeptonExpectation = (TTree*) fExp->Get("mc_exp_tree");

	
  // Set branch addresses.
  
  LostLeptonExpectation->SetBranchStatus("*",0);
  LostLeptonExpectation->SetBranchStatus("HT",1);
  LostLeptonExpectation->SetBranchStatus("MHT",1);
  LostLeptonExpectation->SetBranchStatus("NJets",1);
  LostLeptonExpectation->SetBranchStatus("BTags",1);
  LostLeptonExpectation->SetBranchStatus("Weight",1);
  LostLeptonExpectation->SetBranchStatus("Bin",1);
  LostLeptonExpectation->SetBranchStatus("Expectation",1);
  LostLeptonExpectation->SetBranchStatus("ExpectationReductionIsoTrack",1);
  LostLeptonExpectation->SetBranchStatus("muAcc",1);
  LostLeptonExpectation->SetBranchStatus("muReco",1);
  LostLeptonExpectation->SetBranchStatus("muIso",1);
  LostLeptonExpectation->SetBranchStatus("elecAcc",1);
  LostLeptonExpectation->SetBranchStatus("elecReco",1);
  LostLeptonExpectation->SetBranchStatus("elecIso",1);
  LostLeptonExpectation->SetBranchStatus("isoTracks",1);
  LostLeptonExpectation->SetBranchStatus("ExpectationDiLep",1);


  LostLeptonExpectation->SetBranchAddress("HT",&HT);
  LostLeptonExpectation->SetBranchAddress("MHT",&MHT);
  LostLeptonExpectation->SetBranchAddress("NJets",&NJets);
  LostLeptonExpectation->SetBranchAddress("BTags",&BTags);
  LostLeptonExpectation->SetBranchAddress("Weight",&Weight);
  LostLeptonExpectation->SetBranchAddress("Bin",&Bin);
	
  LostLeptonExpectation->SetBranchAddress("Expectation",&Expectation);
  LostLeptonExpectation->SetBranchAddress("ExpectationReductionIsoTrack",&ExpectationReductionIsoTrack);

  LostLeptonExpectation->SetBranchAddress("muAcc",&muAcc);
  LostLeptonExpectation->SetBranchAddress("muReco",&muReco);
  LostLeptonExpectation->SetBranchAddress("muIso",&muIso);
	
  LostLeptonExpectation->SetBranchAddress("elecAcc",&elecAcc);
  LostLeptonExpectation->SetBranchAddress("elecReco",&elecReco);
  LostLeptonExpectation->SetBranchAddress("elecIso",&elecIso);
	
  LostLeptonExpectation->SetBranchAddress("isoTracks",&isoTracks);
  LostLeptonExpectation->SetBranchAddress("ExpectationDiLep",&ExpectationDiLep);
	

  std::cout<<"Loop on Expectation"<<std::endl;
  Long64_t nentries = LostLeptonExpectation->GetEntries();  

  printf("\n\n Looping over %llu events.\n\n", nentries ) ;

  Long64_t nbytes = 0, nb = 0;

  for (Long64_t jentry=0; jentry<nentries;jentry++) {

    if ( jentry%(nentries/100) == 0 && jentry>0 ) { printf("  %9llu out of %9llu (%2lld%%)\n", jentry, nentries, ((100*jentry)/nentries)+1 ) ; fflush(stdout) ; }

    nb = LostLeptonExpectation->GetEntry(jentry);   nbytes += nb;

    SearchBin = Bin;

    // total expectation
    if(SearchBin > 900) continue;
    if (HT<800&&MHT>750) continue; // to match the standard analysis
    if (NJets<7 || BTags<2) continue; // in case we didn't cut on this earlier
    scaledWeight = Weight * scaleFactorWeight;

    if(Expectation==1 && NJets>6.1)
      {
    	totalExpectation_->Fill(SearchBin, scaledWeight);
    	totalExp+=scaledWeight;
    	totalExpError+= scaledWeight*scaledWeight;
      }
    if(Expectation==1 && ExpectationReductionIsoTrack==0 && NJets>6.1)
      {
    	totalExpectationIsoTrackReduction_->Fill(SearchBin, scaledWeight);
    	totalExpIsoTrack+=scaledWeight;
    	totalExpIsoTrackError+= scaledWeight*scaledWeight;
	totalExp_LL_->Fill(SearchBin, scaledWeight);

	totalExp_HT_LL_->Fill(HT, scaledWeight);
	totalExp_MHT_LL_->Fill(MHT, scaledWeight);
	totalExp_NJets_LL_->Fill(NJets, scaledWeight);
	totalExp_BTags_LL_->Fill(BTags, scaledWeight);

	nEvtsExp_LL_->Fill(SearchBin);


      }
    if(muAcc==0)
      {
    	totalExpectationMuAcc_->Fill(SearchBin, scaledWeight);
    	totalExpMuAcc+=scaledWeight;
      }
    if(muReco==0)
      {
    	totalExpectationMuReco_->Fill(SearchBin, scaledWeight);
    	totalExpMuReco+=scaledWeight;
      }
    if(muIso==0)
      {
    	totalExpectationMuIso_->Fill(SearchBin, scaledWeight);
    	totalExpMuIso+=scaledWeight;
      }
					
    if(elecAcc==0)
      {
    	totalExpectationElecAcc_->Fill(SearchBin, scaledWeight);
    	totalExpElecAcc+=scaledWeight;
      }
    if(elecReco==0)
      {
    	totalExpectationElecReco_->Fill(SearchBin, scaledWeight);
    	totalExpElecReco+=scaledWeight;
      }
    if(elecIso==0)
      {
    	totalExpectationElecIso_->Fill(SearchBin, scaledWeight);
    	totalExpElecIso+=scaledWeight;
      }
  }
	
  std::cout<<"Finished"<<std::endl;

  //Prediction MC Tree
  gROOT->Reset();
  TFile *fPreMC = (TFile*)gROOT->GetListOfFiles()->FindObject(InputPath_Prediction);
  if (!fPreMC) {
    fPreMC = new TFile(InputPath_Prediction);
  }
  TTree* LostLeptonPrediction = (TTree*) fPreMC->Get("pred_tree");

  
  LostLeptonPrediction->SetBranchAddress("HT",&HT);
  LostLeptonPrediction->SetBranchAddress("MHT",&MHT);
  LostLeptonPrediction->SetBranchAddress("NJets",&NJets);
  LostLeptonPrediction->SetBranchAddress("BTags",&BTags);
  LostLeptonPrediction->SetBranchAddress("Weight",&Weight);
  LostLeptonPrediction->SetBranchAddress("Bin",&Bin);
	
  LostLeptonPrediction->SetBranchAddress("MTW",&MTW);
  LostLeptonPrediction->SetBranchAddress("selectedIDIsoMuonsNum",&selectedIDIsoMuonsNum);
  LostLeptonPrediction->SetBranchAddress("selectedIDIsoElectronsNum",&selectedIDIsoElectronsNum);
  LostLeptonPrediction->SetBranchAddress("totalWeightDiLep",&totalWeightDiLep);
  LostLeptonPrediction->SetBranchAddress("totalWeightDiLepIsoTrackReducedCombined",&totalWeightDiLepIsoTrackReducedCombined);
	
  LostLeptonPrediction->SetBranchAddress("muIsoWeight",&muIsoWeight);
  LostLeptonPrediction->SetBranchAddress("muRecoWeight",&muRecoWeight);
  LostLeptonPrediction->SetBranchAddress("muAccWeight",&muAccWeight);
	
  LostLeptonPrediction->SetBranchAddress("elecAccWeight",&elecAccWeight);
  LostLeptonPrediction->SetBranchAddress("elecRecoWeight",&elecRecoWeight);
  LostLeptonPrediction->SetBranchAddress("elecIsoWeight",&elecIsoWeight);

  LostLeptonPrediction->SetBranchStatus("*",0);
  LostLeptonPrediction->SetBranchStatus("HT",1);
  LostLeptonPrediction->SetBranchStatus("MHT",1);
  LostLeptonPrediction->SetBranchStatus("NJets",1);
  LostLeptonPrediction->SetBranchStatus("BTags",1);
  LostLeptonPrediction->SetBranchStatus("Weight",1);
  LostLeptonPrediction->SetBranchStatus("Bin",1);
  LostLeptonPrediction->SetBranchStatus("MTW",1);
  LostLeptonPrediction->SetBranchStatus("selectedIDIsoMuonsNum",1);
  LostLeptonPrediction->SetBranchStatus("selectedIDIsoElectronsNum",1);
  LostLeptonPrediction->SetBranchStatus("totalWeightDiLep",1);
  LostLeptonPrediction->SetBranchStatus("totalWeightDiLepIsoTrackReducedCombined",1);
  LostLeptonPrediction->SetBranchStatus("muIsoWeight",1);
  LostLeptonPrediction->SetBranchStatus("muRecoWeight",1);
  LostLeptonPrediction->SetBranchStatus("muAccWeight",1);
  LostLeptonPrediction->SetBranchStatus("elecAccWeight",1);
  LostLeptonPrediction->SetBranchStatus("elecRecoWeight",1);
  LostLeptonPrediction->SetBranchStatus("elecIsoWeight",1);

  std::cout<<"Loop on Prediction (MC)"<<std::endl;
  
  nentries = LostLeptonPrediction->GetEntries();
  nbytes = 0;
  nb=0;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {

    if ( jentry%(nentries/100) == 0 && jentry>0 ) { printf("  %9llu out of %9llu (%2lld%%)\n", jentry, nentries, ((100*jentry)/nentries)+1 ) ; fflush(stdout) ; }

    nb = LostLeptonPrediction->GetEntry(jentry);   nbytes += nb;

    SearchBin = Bin;
    if(SearchBin > 900) continue;
    if (HT<800&&MHT>750) continue; // to match the standard analysis
    if (NJets<7 || BTags<2) continue; // in case we didn't cut on this earlier

    if(selectedIDIsoMuonsNum!=1||selectedIDIsoElectronsNum!=0)continue; // single-muon control sample
    if(MTW>100)continue;

    scaledWeight = Weight * scaleFactorWeight;

    totalPred_LL_MC_->Fill(SearchBin, totalWeightDiLepIsoTrackReducedCombined*scaleFactorWeight);
    totalPred_HT_LL_MC_->Fill(HT, totalWeightDiLepIsoTrackReducedCombined*scaleFactorWeight);
    totalPred_MHT_LL_MC_->Fill(MHT, totalWeightDiLepIsoTrackReducedCombined*scaleFactorWeight);
    totalPred_NJets_LL_MC_->Fill(NJets, totalWeightDiLepIsoTrackReducedCombined*scaleFactorWeight);
    totalPred_BTags_LL_MC_->Fill(BTags, totalWeightDiLepIsoTrackReducedCombined*scaleFactorWeight);

    totalCS_LL_MC_->Fill(SearchBin, scaledWeight);
    nEvtsCS_LL_MC_->Fill(SearchBin);

    avgWeight_LL_MC_->Fill(SearchBin, abs(totalWeightDiLepIsoTrackReducedCombined/Weight), Weight);
    avgWeight_HT_LL_MC_->Fill(HT, abs(totalWeightDiLepIsoTrackReducedCombined/Weight), Weight);
    avgWeight_MHT_LL_MC_->Fill(MHT, abs(totalWeightDiLepIsoTrackReducedCombined/Weight), Weight);
    avgWeight_NJets_LL_MC_->Fill(NJets, abs(totalWeightDiLepIsoTrackReducedCombined/Weight), Weight);
    avgWeight_BTags_LL_MC_->Fill(BTags, abs(totalWeightDiLepIsoTrackReducedCombined/Weight), Weight);

  }    

  std::cout<<"Finished"<<std::endl;

  //Prediction Data Tree
  gROOT->Reset();
  TFile *fPre = (TFile*)gROOT->GetListOfFiles()->FindObject(InputPath_Prediction_Data);
  if (!fPre) {
    fPre = new TFile(InputPath_Prediction_Data);
  }
  TTree* LostLeptonPredictionData = (TTree*) fPre->Get("pred_tree");

  LostLeptonPredictionData->SetBranchAddress("HT",&HT);
  LostLeptonPredictionData->SetBranchAddress("MHT",&MHT);
  LostLeptonPredictionData->SetBranchAddress("NJets",&NJets);
  LostLeptonPredictionData->SetBranchAddress("BTags",&BTags);
  LostLeptonPredictionData->SetBranchAddress("Weight",&Weight);
  LostLeptonPredictionData->SetBranchAddress("Bin",&Bin);
	
  LostLeptonPredictionData->SetBranchAddress("MTW",&MTW);
  LostLeptonPredictionData->SetBranchAddress("selectedIDIsoMuonsNum",&selectedIDIsoMuonsNum);
  LostLeptonPredictionData->SetBranchAddress("selectedIDIsoElectronsNum",&selectedIDIsoElectronsNum);
  LostLeptonPredictionData->SetBranchAddress("totalWeightDiLep",&totalWeightDiLep);
  LostLeptonPredictionData->SetBranchAddress("totalWeightDiLepIsoTrackReducedCombined",&totalWeightDiLepIsoTrackReducedCombined);
	
  LostLeptonPredictionData->SetBranchAddress("muIsoWeight",&muIsoWeight);
  LostLeptonPredictionData->SetBranchAddress("muRecoWeight",&muRecoWeight);
  LostLeptonPredictionData->SetBranchAddress("muAccWeight",&muAccWeight);
	
  LostLeptonPredictionData->SetBranchAddress("elecAccWeight",&elecAccWeight);
  LostLeptonPredictionData->SetBranchAddress("elecRecoWeight",&elecRecoWeight);
  LostLeptonPredictionData->SetBranchAddress("elecIsoWeight",&elecIsoWeight);
  
  LostLeptonPredictionData->SetBranchStatus("*",0);
  LostLeptonPredictionData->SetBranchStatus("HT",1);
  LostLeptonPredictionData->SetBranchStatus("MHT",1);
  LostLeptonPredictionData->SetBranchStatus("NJets",1);
  LostLeptonPredictionData->SetBranchStatus("BTags",1);
  LostLeptonPredictionData->SetBranchStatus("Weight",1);
  LostLeptonPredictionData->SetBranchStatus("Bin",1);
  LostLeptonPredictionData->SetBranchStatus("MTW",1);
  LostLeptonPredictionData->SetBranchStatus("selectedIDIsoMuonsNum",1);
  LostLeptonPredictionData->SetBranchStatus("selectedIDIsoElectronsNum",1);
  LostLeptonPredictionData->SetBranchStatus("totalWeightDiLep",1);
  LostLeptonPredictionData->SetBranchStatus("totalWeightDiLepIsoTrackReducedCombined",1);
  LostLeptonPredictionData->SetBranchStatus("muIsoWeight",1);
  LostLeptonPredictionData->SetBranchStatus("muRecoWeight",1);
  LostLeptonPredictionData->SetBranchStatus("muAccWeight",1);
  LostLeptonPredictionData->SetBranchStatus("elecAccWeight",1);
  LostLeptonPredictionData->SetBranchStatus("elecRecoWeight",1);
  LostLeptonPredictionData->SetBranchStatus("elecIsoWeight",1);


  std::cout<<"Loop on Prediction (Data)"<<std::endl;
	
  nentries = LostLeptonPredictionData->GetEntries();
  cout << nentries << endl;
  nbytes = 0;
  nb=0;
  double scaleMC = 1.0;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    //printf("Entry %lld\n", jentry);

    //    if ( jentry%(nentries/100) == 0 && jentry>0 ) { printf("  %9llu out of %9llu (%2lld%%)\n", jentry, nentries, ((100*jentry)/nentries)+1 ) ; fflush(stdout) ; }
    nb = LostLeptonPredictionData->GetEntry(jentry);   nbytes += nb;
    //printf("nb/nbytes: %lld/%lld\n", nb, nbytes);
    if(!useMCForDataTree && InputPath_Prediction_Data != InputPath_Prediction) scaleFactorWeight = 1.0;
    //    printf("Weight/scaleFactorWeight/scaleMC: %f/%f/%f\n", Weight, scaleFactorWeight, scaleMC);

    SearchBin = Bin;
    //printf("Search bin %d\n", SearchBin);

    if(SearchBin > 900) continue;
    if (HT<800&&MHT>750) continue; // to match the standard analysis
    if(selectedIDIsoMuonsNum!=1||selectedIDIsoElectronsNum!=0)continue; // single-muon control sample
    if (NJets<7 || BTags<2) continue; // in case we didn't cut on these earlier
    if(MTW>100)continue;
    scaledWeight = Weight * scaleFactorWeight;

    if(InputPath_Prediction_Data != InputPath_Prediction) scaleMC = Weight;

    
    totalPred_LL_->Fill(SearchBin, totalWeightDiLepIsoTrackReducedCombined*scaleFactorWeight/scaleMC);
    totalPred_HT_LL_->Fill(HT, totalWeightDiLepIsoTrackReducedCombined*scaleFactorWeight/scaleMC);
    totalPred_MHT_LL_->Fill(MHT, totalWeightDiLepIsoTrackReducedCombined*scaleFactorWeight/scaleMC);
    totalPred_NJets_LL_->Fill(NJets, totalWeightDiLepIsoTrackReducedCombined*scaleFactorWeight/scaleMC);
    totalPred_BTags_LL_->Fill(BTags, totalWeightDiLepIsoTrackReducedCombined*scaleFactorWeight/scaleMC);


    totalCS_LL_->Fill(SearchBin, scaledWeight/scaleMC);
    nEvtsCS_LL_->Fill(SearchBin);

    avgWeight_LL_->Fill(SearchBin, abs(totalWeightDiLepIsoTrackReducedCombined/Weight), Weight);


    ControlSampleMu_->Fill(SearchBin, scaledWeight/scaleMC);
    			
    totalPrediction_->Fill(SearchBin, totalWeightDiLep*scaleFactorWeight/scaleMC);
    totalPre+=totalWeightDiLep*scaleFactorWeight/scaleMC;
    totalPreError+= totalWeightDiLep*scaleFactorWeight*totalWeightDiLep*scaleFactorWeight/scaleMC/scaleMC;
    // isotrackveto applied
    totalPredictionIsoTrackReduction_->Fill(SearchBin, totalWeightDiLepIsoTrackReducedCombined*scaleFactorWeight/scaleMC);
    totalPreIsoTrack+=totalWeightDiLepIsoTrackReducedCombined*scaleFactorWeight/scaleMC;
    totalPreIsoTrackError+=totalWeightDiLepIsoTrackReducedCombined*scaleFactorWeight*totalWeightDiLepIsoTrackReducedCombined*scaleFactorWeight/scaleMC/scaleMC;
  
    // separated components
    totalPredictionMuCSMuAcc_->Fill(SearchBin, muAccWeight*scaleFactorWeight/scaleMC);
    totalPredictionMuCSMuReco_->Fill(SearchBin, muRecoWeight*scaleFactorWeight/scaleMC);
    totalPredictionMuCSMuIso_->Fill(SearchBin, muIsoWeight*scaleFactorWeight/scaleMC);
    totalPreMuAcc+=muAccWeight*scaleFactorWeight/scaleMC;
    totalPreMuReco+=muRecoWeight*scaleFactorWeight/scaleMC;
    totalPreMuIso+=muIsoWeight*scaleFactorWeight/scaleMC;
    			
    totalPredictionMuCSElecAcc_->Fill(SearchBin, elecAccWeight*scaleFactorWeight/scaleMC);
    totalPredictionMuCSElecReco_->Fill(SearchBin, elecRecoWeight*scaleFactorWeight/scaleMC);
    totalPredictionMuCSElecIso_->Fill(SearchBin, elecIsoWeight*scaleFactorWeight/scaleMC);
    totalPreElecAcc+=elecAccWeight*scaleFactorWeight/scaleMC;
    totalPreElecReco+=elecRecoWeight*scaleFactorWeight/scaleMC;
    totalPreElecIso+=elecIsoWeight*scaleFactorWeight/scaleMC;
    
 
  }

  std::cout<<"Finished"<<std::endl;

	
  TH1D* FullClosure = (TH1D*) totalExpectationIsoTrackReduction_->Clone();
  FullClosure->Divide(totalPredictionIsoTrackReduction_,totalExpectationIsoTrackReduction_,1,1,"");
  FullClosure->SetName("LostLeptonClosure");
  FullClosure->SetTitle("Closure of lost-lepton method");
		

  TH1D* ClosureWoIsoTrack = (TH1D*) totalExpectation_->Clone();
  ClosureWoIsoTrack->Divide(totalPrediction_,totalExpectation_,1,1,"");
  ClosureWoIsoTrack->SetName("ClosureWoIsoTrack");
  ClosureWoIsoTrack->SetTitle("Closure of lost-lepton method (wo IsotrackVeto)");


  std::cout<<"--------------------------------------------------------------------------------------------------------------------------\n";
  std::cout<<"\n Summary:"<<std::endl;
  std::cout<<"TotalExpectationIsoTrack: "<<totalExpIsoTrack<<" +- " << sqrt(totalExpIsoTrackError)<<"\n TotalPredictionIsoTrack: "<<totalPreIsoTrack<<" +- "<<sqrt(totalPreIsoTrackError)<<std::endl;
  std::cout<<"TotalExpectation: "<<totalExp<<" +- " << sqrt(totalExpError)<<"\n TotalPrediction: "<<totalPre<<" +- "<<sqrt(totalPreError)<<std::endl;
  std::cout<<"MuAccExp: "<<totalExpMuAcc<<"\n MuAccPre: "<<totalPreMuAcc<<std::endl;
  std::cout<<"MuRecoExp: "<<totalExpMuReco<<"\n MuRecoPre: "<<totalPreMuReco<<std::endl;
  std::cout<<"MuIsoExp: "<<totalExpMuIso<<"\n MuIsoPre: "<<totalPreMuIso<<std::endl;
  std::cout<<"ElecAccExp: "<<totalExpElecAcc<<"\n ElecAccPre: "<<totalPreElecAcc<<std::endl;
  std::cout<<"ElecRecoExp: "<<totalExpElecReco<<"\n ElecRecoPre: "<<totalPreElecReco<<std::endl;
  std::cout<<"ElecIsoExp: "<<totalExpElecIso<<"\n ElecIsoPre: "<<totalPreElecIso<<std::endl;
  std::cout<<"--------------------------------------------------------------------------------------------------------------------------\n";


	
  outPutFile->cd();
  FullClosure->Write();
  ClosureWoIsoTrack->Write();
  outPutFile->mkdir("Closures");
  TDirectory *dClosures = (TDirectory*)outPutFile->Get("Closures");

  SaveClosure(totalPredictionMuCSMuAcc_, totalExpectationMuAcc_, dClosures);
  SaveClosure(totalPredictionMuCSMuReco_, totalExpectationMuReco_, dClosures);
  SaveClosure(totalPredictionMuCSMuIso_, totalExpectationMuIso_, dClosures);
	
  SaveClosure(totalPredictionMuCSElecAcc_, totalExpectationElecAcc_, dClosures);
  SaveClosure(totalPredictionMuCSElecReco_, totalExpectationElecReco_, dClosures);
  SaveClosure(totalPredictionMuCSElecIso_, totalExpectationElecIso_, dClosures);
	
  SaveClosure(totalPredictionIsoTrackReduction_, totalExpectationIsoTrackReduction_, dClosures);
	
  outPutFile->mkdir("Expectation");
  TDirectory *dExpectation = (TDirectory*)outPutFile->Get("Expectation");
  dExpectation->cd();
  totalExpectation_->Write();
  outPutFile->mkdir("Prediction");
  TDirectory *dPrediction = (TDirectory*)outPutFile->Get("Prediction");
  dPrediction->cd();
  totalPrediction_->Write();
	
  outPutFile->mkdir("ControlSample");
  TDirectory *dControlSample = (TDirectory*)outPutFile->Get("ControlSample");
  dControlSample->cd();
  ControlSampleMu_->Write();



  TFile *LLoutPutFile = new TFile(OutputPath_Prediction,"RECREATE");

  LLoutPutFile->cd();
  LLoutPutFile->mkdir("SearchVariables");
  TDirectory *dSearchVariables = (TDirectory*)LLoutPutFile->Get("SearchVariables");
  dSearchVariables->cd();

  totalExp_HT_LL_->Write();
  totalExp_MHT_LL_->Write();
  totalExp_NJets_LL_->Write();
  totalExp_BTags_LL_->Write();

  totalPred_HT_LL_MC_->Write();
  totalPred_MHT_LL_MC_->Write();
  totalPred_NJets_LL_MC_->Write();
  totalPred_BTags_LL_MC_->Write();

  totalPred_HT_LL_->Write();
  totalPred_MHT_LL_->Write();
  totalPred_NJets_LL_->Write();
  totalPred_BTags_LL_->Write();

  LLoutPutFile->cd();
  LLoutPutFile->mkdir("Closure");
  TDirectory *dClosure = (TDirectory*)LLoutPutFile->Get("Closure");
  dClosure->cd();

  TH1D *ClosureTest = (TH1D*) totalExp_LL_->Clone("ClosureTest");
  ClosureTest->Divide(totalPred_LL_MC_);
  ClosureTest->SetTitle("Expectation / Prediction");
  SetBinLabel(ClosureTest);
  ClosureTest->Write();

  TH1D *ClosureTest_HT = (TH1D*) totalExp_HT_LL_->Clone("ClosureTest_HT");
  ClosureTest_HT->Divide(totalPred_HT_LL_MC_);
  ClosureTest_HT->SetTitle("Expectation / Prediction");
  ClosureTest_HT->Write();

  TH1D *ClosureTest_MHT = (TH1D*) totalExp_MHT_LL_->Clone("ClosureTest_MHT");
  ClosureTest_MHT->Divide(totalPred_MHT_LL_MC_);
  ClosureTest_MHT->SetTitle("Expectation / Prediction");
  ClosureTest_MHT->Write();

  TH1D *ClosureTest_NJets = (TH1D*) totalExp_NJets_LL_->Clone("ClosureTest_NJets");
  ClosureTest_NJets->Divide(totalPred_NJets_LL_MC_);
  ClosureTest_NJets->SetTitle("Expectation / Prediction");
  ClosureTest_NJets->Write();

  TH1D *ClosureTest_BTags = (TH1D*) totalExp_BTags_LL_->Clone("ClosureTest_BTags");
  ClosureTest_BTags->Divide(totalPred_BTags_LL_MC_);
  ClosureTest_BTags->SetTitle("Expectation / Prediction");
  ClosureTest_BTags->Write();

  // TH1D *nonClosureSysUp = (TH1D*) ClosureTest->Clone("nonClosureSysUp");
  // nonClosureSysUp->Reset();
  // nonClosureSysUp->SetTitle("nonClosureSysUp");
  // TH1D *nonClosureSysDown = (TH1D*) ClosureTest->Clone("nonClosureSysDown");
  // nonClosureSysDown->Reset();
  // nonClosureSysDown->SetTitle("nonClosureSysUp");

  LLoutPutFile->cd();
  LLoutPutFile->mkdir("Expectation");
  TDirectory *dExp = (TDirectory*)LLoutPutFile->Get("Expectation");
  dExp->cd();

  SetBinLabel(totalExp_LL_);
  totalExp_LL_->Write();
  SetBinLabel(nEvtsExp_LL_);
  nEvtsExp_LL_->Write();

  LLoutPutFile->cd();
  LLoutPutFile->mkdir("Prediction_data");
  TDirectory *dPreData = (TDirectory*)LLoutPutFile->Get("Prediction_data");
  dPreData->cd();

  SetBinLabel(totalPred_LL_);
  totalPred_LL_->Write();

  for(int i = 0; i<=ClosureTest->GetNbinsX()+1; ++i){
    totalPredNonClosureUp_LL_->SetBinContent(i, 1.+min(1., max(abs(ClosureTest->GetBinContent(i)-1.), ClosureTest->GetBinError(i))));
    totalPredNonClosureDown_LL_->SetBinContent(i, 1.-min(1., max(abs(ClosureTest->GetBinContent(i)-1.), ClosureTest->GetBinError(i))));
  }
  SetBinLabel(totalPredNonClosureUp_LL_);
  totalPredNonClosureUp_LL_->Write();
  SetBinLabel(totalPredNonClosureDown_LL_);
  totalPredNonClosureDown_LL_->Write();

  SetBinLabel(totalCS_LL_);
  totalCS_LL_->Write();
  SetBinLabel(nEvtsCS_LL_);
  nEvtsCS_LL_->Write();

  SetBinLabel(avgWeight_LL_);
  avgWeight_LL_->Write();

  LLoutPutFile->cd();
  LLoutPutFile->mkdir("Prediction_MC");
  TDirectory *dPreMC = (TDirectory*)LLoutPutFile->Get("Prediction_MC");
  dPreMC->cd();

  SetBinLabel(totalPred_LL_MC_);
  totalPred_LL_MC_->Write();



  for(int i = 0; i<=ClosureTest->GetNbinsX()+1; ++i){
    totalPredNonClosureUp_LL_MC_->SetBinContent(i, 1.+min(1., max(abs(ClosureTest->GetBinContent(i)-1.), ClosureTest->GetBinError(i))));
    totalPredNonClosureDown_LL_MC_->SetBinContent(i, 1.-min(1., max(abs(ClosureTest->GetBinContent(i)-1.), ClosureTest->GetBinError(i))));
  }
  SetBinLabel(totalPredNonClosureUp_LL_MC_);
  totalPredNonClosureUp_LL_MC_->Write();
  SetBinLabel(totalPredNonClosureDown_LL_MC_);
  totalPredNonClosureDown_LL_MC_->Write();

  SetBinLabel(totalCS_LL_MC_);
  totalCS_LL_MC_->Write();
  SetBinLabel(nEvtsCS_LL_MC_);
  nEvtsCS_LL_MC_->Write();

  SetBinLabel(avgWeight_LL_MC_);
  avgWeight_LL_MC_->Write();

  LLoutPutFile->cd();
  LLoutPutFile->mkdir("AvgWeight_MC");
  TDirectory *dAvgWeight = (TDirectory*)LLoutPutFile->Get("AvgWeight_MC");
  dAvgWeight->cd();

  SetBinLabel(avgWeight_LL_MC_);
  avgWeight_LL_MC_->Write();

  avgWeight_HT_LL_MC_->Write();
  avgWeight_MHT_LL_MC_->Write();
  avgWeight_NJets_LL_MC_->Write();
  avgWeight_BTags_LL_MC_->Write();


  LLoutPutFile->cd();
  LLoutPutFile->mkdir("Prediction_Data_MC");
  TDirectory *dDataMC = (TDirectory*)LLoutPutFile->Get("Prediction_Data_MC");
  dDataMC->cd();

  TH1D *Prediction_Data_MC = (TH1D*) totalPred_LL_->Clone("Prediction_Data_MC");
  Prediction_Data_MC->Divide(totalPred_LL_MC_);
  Prediction_Data_MC->SetTitle("Prediction Data / Prediction MC");
  SetBinLabel(Prediction_Data_MC);
  Prediction_Data_MC->Write();


  LLoutPutFile->Close();

	

}


void SaveClosure(TH1D* prediction, TH1D* expectation, TDirectory* Folder) // prediction durch expectation
{
  TH1D* Closure = (TH1D*) prediction->Clone();
  Closure->Divide(prediction,expectation,1,1,"B");
  TString title = prediction->GetTitle();
  title +="_closure";
  //  title = "#mu & e Control-Sample Ratio in Search Bins; Search bins; #mu / e CS";
  Closure->SetTitle(title);
  title = prediction->GetName();
  title+="_closure";
  Closure->SetName(title);
  Folder->cd();
  Closure->Write();
}

void SetBinLabel(TH1D* hist){
  hist->GetXaxis()->SetBinLabel(1, "NJets1-BTags1-MHT1");
  hist->GetXaxis()->SetBinLabel(2, "NJets1-BTags2-MHT1");
  hist->GetXaxis()->SetBinLabel(3, "NJets1-BTags12-MHT2");
  hist->GetXaxis()->SetBinLabel(4, "NJets2-BTags1-MHT1");
  hist->GetXaxis()->SetBinLabel(5, "NJets2-BTags2-MHT1");
  hist->GetXaxis()->SetBinLabel(6, "NJets2-BTags12-MHT2");
  hist -> GetXaxis() -> LabelsOption("v");
}

void SaveFraction(TH1D* Top, TH1D* Bottom, TDirectory* dir){
  for(int i = 1; i<Bottom->GetNbinsX()+1; ++i){
    if(Bottom->GetBinContent(i)>0) Top->SetBinContent(i, 1.+Top->GetBinContent(i)/Bottom->GetBinContent(i));
    else Top->SetBinContent(i, -999);
    //else Top->SetBinContent(i, 0);
    Top->SetBinError(i, 0);
  } 

  SetBinLabel(Top);

  dir->cd();
  Top->Write();
}

void addUncertainties(TH1D* total, std::vector<TH1D*> uncertainties, bool upperUnc){
  for(int i = 0; i <= total->GetNbinsX(); i++){
    Double_t sumSq = 0.;

    for (std::vector<TH1D*>::iterator it = uncertainties.begin() ; it != uncertainties.end(); ++it){
      sumSq += (*it)->GetBinContent(i) * (*it)->GetBinContent(i);
    }

    if(upperUnc) total->SetBinContent(i, std::sqrt(sumSq));
    else total->SetBinContent(i, -std::sqrt(sumSq));
  }

}

// old code for printing search bin information

  // SearchBins *SearchBins_ = new SearchBins(true);

  // double LLexpErr = 0;
  // double LLexp = totalExp_LL_->IntegralAndError(1, 6, LLexpErr);
  // double LLpreErr = 0;
  // double LLpre = totalPred_LL_->IntegralAndError(1, 6, LLpreErr);

  //printf("Total: & & & & & & & $%3.3f\\pm$%3.3f & $%3.3f\\pm$%3.3f \\\\\n", LLexp, LLexpErr, LLpre, LLpreErr);

  //if(!useMCForDataTree && InputPath_Prediction_Data == InputPath_Prediction) std::cout<<"ATTENTION: Full MC statistics used to do prediction! Only approx. stat. unc. (~sqrt(n)) shown on prediction!"<<std::endl;

  // if(InputPath_Prediction_Data != InputPath_Prediction) printf("Bin & NJets & BTags & HT & MHT & CS\\_MC (nEntries) & avg. weight (MC) [$\\pm$ stat. $\\pm$ statEff. $\\pm$ sysEff. $\\pm$ nonClos.] & CS\\_data & avg. weight (data) [$\\pm$ stat. $\\pm$ statEff. $\\pm$ sysEff. $\\pm$ nonClos.] & Prediction (data) [$\\pm$ stat. $\\pm$ statEff. $\\pm$ sysEff. $\\pm$ nonClos.] & Expectation \\\\\n");
  // else printf("Bin & NJets & BTags & HT & MHT & CS\\_MC (nEntries) & avg. weight (MC) [$\\pm$ stat. $\\pm$ statEff. $\\pm$ sysEff. $\\pm$ nonClos.] & Prediction (MC) [$\\pm$ stat. $\\pm$ statEff. $\\pm$ sysEff. $\\pm$ nonClos.] & Expectation \\\\\n");

  // for(int i = 1; i<totalPred_LL_->GetNbinsX()+1; ++i){

  //   //SearchBin (Number, NJets, BTags, HT, MHT)
  //   printf("%1.0i & ", i);

  //   if(SearchBins_->GetSearchBin(i-1)->NJetsmin_<SearchBins_->GetSearchBin(i-1)->NJetsmax_ && SearchBins_->GetSearchBin(i-1)->NJetsmin_>=0){
  //     printf("%d-", SearchBins_->GetSearchBin(i-1)->NJetsmin_);
  //     if(SearchBins_->GetSearchBin(i-1)->NJetsmax_<9999) printf("%d & ", SearchBins_->GetSearchBin(i-1)->NJetsmax_);
  //     else printf("Inf & ");
  //   }else{
  //     printf("%d & ", SearchBins_->GetSearchBin(i-1)->NJetsmax_);
  //   }

  //   if(SearchBins_->GetSearchBin(i-1)->BTagsmin_<SearchBins_->GetSearchBin(i-1)->BTagsmax_ && SearchBins_->GetSearchBin(i-1)->BTagsmin_>=0){
  //     printf("%d-",SearchBins_->GetSearchBin(i-1)->BTagsmin_);
  //     if(SearchBins_->GetSearchBin(i-1)->BTagsmax_<9999) printf("%d & ",SearchBins_->GetSearchBin(i-1)->BTagsmax_);
  //     else printf("Inf & ");
  //   }else{
  //     printf("%d & ", SearchBins_->GetSearchBin(i-1)->BTagsmax_);
  //   }

  //   printf("%3.0f-",SearchBins_->GetSearchBin(i-1)->HTmin_);
  //   if(SearchBins_->GetSearchBin(i-1)->HTmax_<9999) printf("%3.0f & ",SearchBins_->GetSearchBin(i-1)->HTmax_);
  //   else printf("Inf & ");

  //   printf("%3.0f-",SearchBins_->GetSearchBin(i-1)->MHTmin_);
  //   if(SearchBins_->GetSearchBin(i-1)->MHTmax_<9999) printf("%3.0f & ",SearchBins_->GetSearchBin(i-1)->MHTmax_);
  //   else printf("Inf & ");

  //   // CS events (MC)
  //   printf("%3.3f & ", totalCS_LL_MC_->GetBinContent(i));
  //   //printf("%3.3f (%1.0f) & ", totalCS_LL_MC_->GetBinContent(i), nEvtsCS_LL_MC_->GetBinContent(i));
 
  //   // Average weight per Bin (MC)
  //   printf("$%3.3f\\pm%3.3f^{+%3.3f}_{%3.3f}$ & ", avgWeight_LL_MC_->GetBinContent(i), avgWeight_LL_MC_->GetBinError(i), (totalPredNonClosureUp_LL_MC_->GetBinContent(i)-1)*avgWeight_LL_MC_->GetBinContent(i), (totalPredNonClosureDown_LL_MC_->GetBinContent(i)-1)*avgWeight_LL_MC_->GetBinContent(i));
    
  //   // CS events (data)
  //   if(InputPath_Prediction_Data != InputPath_Prediction) printf("%1.0f & ", totalCS_LL_->GetBinContent(i));

  //   // Average weight per Bin (data)
  //   //if(InputPath_Prediction_Data != InputPath_Prediction) printf("$%3.3f\\pm%3.3f^{+%3.3f}_{-%3.3f}{}^{+%3.3f}_{-%3.3f}{}^{+%3.3f}_{-%3.3f}$ & ", avgWeight_LL_->GetBinContent(i), avgWeight_LL_->GetBinError(i), avgWeightStatUp_LL_->GetBinContent(i), avgWeightStatDown_LL_->GetBinContent(i), avgWeightSysUp_LL_->GetBinContent(i), avgWeightSysDown_LL_->GetBinContent(i), avgWeightNonClosureUp_LL_->GetBinContent(i), avgWeightNonClosureDown_LL_->GetBinContent(i));

  //   // Prediction
  //   // Correct estimate of stat. uncertainty on prediction only possible if data is used or limited MC statistics (e.g. number of events corresponding to 3fb-1)
  //   // For approximation of stat. uncertainty on prediction using full MC statistics use:
  //   //if(!useMCForDataTree && InputPath_Prediction_Data == InputPath_Prediction && approxStatUncertainty) if(totalCS_LL_->GetBinContent(i)>0.00001) totalPred_LL_->SetBinError(i, sqrt(totalPred_LL_->GetBinContent(i)*totalPred_LL_->GetBinContent(i)/totalCS_LL_->GetBinContent(i)));

  //   printf("$%3.3f\\pm%3.3f^{+%3.3f}_{%3.3f}$ & ", totalPred_LL_->GetBinContent(i), totalPred_LL_->GetBinError(i), (totalPredNonClosureUp_LL_->GetBinContent(i)-1)*totalPred_LL_->GetBinContent(i), (totalPredNonClosureDown_LL_->GetBinContent(i)-1)*totalPred_LL_->GetBinContent(i));

  //   // Expectation
  //   printf("$%3.3f\\pm%3.3f$ \\\\\n", totalExp_LL_->GetBinContent(i), totalExp_LL_->GetBinError(i));

  // }
