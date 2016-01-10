#define PredictionMaker_cxx
#include "PredictionMaker.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void PredictionMaker::Run(std::string effFileName, std::string outputFileName, double HTgen_cut)
{

  // load MC efficiencies from previous step
  TFile *effInput = new TFile(effFileName.c_str(),"READ");
  TDirectory *EffInputFolder =   (TDirectory*)effInput->Get("Efficiencies");
  MuIsoActivityPT_ = new TH2Eff("MuIsoActivityPT", EffInputFolder);
  MuRecoActivityPT_= new TH2Eff("MuRecoActivityPT", EffInputFolder);
  MuAccHTMHT_NJets78_ = new TH2Eff("MuAccHTMHT_NJets78", EffInputFolder);
  MuAccHTMHT_NJets9Inf_ = new TH2Eff("MuAccHTMHT_NJets9Inf", EffInputFolder);
  ElecIsoActivityPT_ = new TH2Eff("ElecIsoActivityPT", EffInputFolder);
  ElecRecoActivityPT_= new TH2Eff("ElecRecoActivityPT", EffInputFolder);
  ElecAccHTMHT_NJets78_ = new TH2Eff("ElecAccHTMHT_NJets78", EffInputFolder);
  ElecAccHTMHT_NJets9Inf_ = new TH2Eff("ElecAccHTMHT_NJets9Inf", EffInputFolder);

  MuMTWHTNJets_ = new TH2Eff("MuMTWHTNJets", EffInputFolder);
  MuDiLepContributionMTWAppliedNJets_ = new TH1Eff("MuDiLepContributionMTWNJets1D", EffInputFolder);
  MuDiLepEffMTWAppliedNJets_ = new TH1Eff("MuDiLepMTWNJets1D", EffInputFolder);

  ExpectationReductionMuIsoTrackHTMHT_NJetsHighEff_ = new TH2Eff("MuIsoTrackReductionHTMHT_NJetsHigh", EffInputFolder);
  ExpectationReductionElecIsoTrackHTMHT_NJetsHighEff_ = new TH2Eff("ElecIsoTrackReductionHTMHT_NJetsHigh", EffInputFolder);
  ExpectationReductionPionIsoTrackHTMHT_NJetsHighEff_ = new TH2Eff("PionIsoTrackReductionHTMHT_NJetsHigh", EffInputFolder);

    // TProfiles
  MuMeanWeight_ = new TProfile("MuMeanWeight","MuMeanWeight",6,1.,7.);
  MuMeanWeight_->Sumw2();
  for(int b = 0; b <6; b++){
    MuWeightPerBin_[b] = new TH1D(TString("MuWeightPerBin_")+TString(to_string(b+1)),TString("MuWeightPerBin_")+TString(to_string(b+1)),60,0.,3.0);
    MuWeightPerBin_[b]->Sumw2();
  }  

  TFile *outPutFile = new TFile(outputFileName.c_str(),"RECREATE"); ;
  outPutFile->cd();
  // setup output tree
  tPrediction_ = new TTree("pred_tree","tree for calculating prediction from data or MC");

  tPrediction_->Branch("HT",&HT);
  tPrediction_->Branch("MHT",&MHT);
  tPrediction_->Branch("NJets",&NJets);
  tPrediction_->Branch("BTags",&BTags);
  tPrediction_->Branch("NVtx",&NVtx);
  tPrediction_->Branch("Bin",&Bin_);
  tPrediction_->Branch("isoTracks",&isoTracks);
  //  tPrediction_->Branch("Leptons",&Leptons);
  tPrediction_->Branch("genHT",&genHT);
  tPrediction_->Branch("DeltaPhi1",&DeltaPhi1);
  tPrediction_->Branch("DeltaPhi2",&DeltaPhi2);
  tPrediction_->Branch("DeltaPhi3",&DeltaPhi3);
  tPrediction_->Branch("DeltaPhi4",&DeltaPhi4);
  tPrediction_->Branch("Weight", &Weight);
  tPrediction_->Branch("MET",&METPt);
  tPrediction_->Branch("METPhi",&METPhi);
  tPrediction_->Branch("selectedIDIsoMuonsNum",&selectedIDIsoMuonsNum_);
  tPrediction_->Branch("Muons", "std::vector<TLorentzVector>", &Muons, 32000, 0);
  tPrediction_->Branch("selectedIDIsoMuons_MTW", &selectedIDIsoMuons_MTW);
  tPrediction_->Branch("selectedIDIsoMuons_MT2Activity", &selectedIDIsoMuons_MT2Activity);
  tPrediction_->Branch("selectedIDIsoElectronsNum",&selectedIDIsoElectronsNum_);
  tPrediction_->Branch("Electrons", "std::vector<TLorentzVector>", &Electrons, 32000, 0);
  tPrediction_->Branch("selectedIDIsoElectrons_MTW", &selectedIDIsoElectrons_MTW);
  tPrediction_->Branch("selectedIDIsoElectrons_MT2Activity", &selectedIDIsoElectrons_MT2Activity);
  tPrediction_->Branch("MTW",&MTW);
  tPrediction_->Branch("muMTWEff",&muMTWEff_);
  tPrediction_->Branch("mtwCorrectedWeight",&mtwCorrectedWeight_);
  tPrediction_->Branch("muDiLepContributionMTWAppliedEff",&muDiLepContributionMTWAppliedEff_);
  tPrediction_->Branch("mtwDiLepCorrectedWeight",&mtwDiLepCorrectedWeight_);
  //  tPrediction_->Branch("totalWeightDiLepIsoTrackReduced",&totalWeightDiLepIsoTrackReduced_);
  tPrediction_->Branch("totalWeightDiLepIsoMuTrackReduced",&totalWeightDiLepIsoMuTrackReduced_);
  tPrediction_->Branch("totalWeightDiLepIsoElecTrackReduced",&totalWeightDiLepIsoElecTrackReduced_);
  tPrediction_->Branch("totalWeightDiLepIsoPionTrackReduced",&totalWeightDiLepIsoPionTrackReduced_);
  tPrediction_->Branch("totalWeightDiLepIsoTrackReducedCombined",&totalWeightDiLepIsoTrackReducedCombined_);
  tPrediction_->Branch("muIsoWeight",&muIsoWeight_);
  tPrediction_->Branch("muIsoEff",&muIsoEff_);
  tPrediction_->Branch("muRecoWeight",&muRecoWeight_);
  tPrediction_->Branch("muRecoEff",&muRecoEff_);
  tPrediction_->Branch("muAccWeight",&muAccWeight_);
  tPrediction_->Branch("muAccEff",&muAccEff_);
  tPrediction_->Branch("muTotalWeight",&muTotalWeight_);
  tPrediction_->Branch("muTotalWeightDiLep",&muTotalWeightDiLep_);
  tPrediction_->Branch("muTotalWeightDiLepIsoTrackReduced",&muTotalWeightDiLepIsoTrackReduced_);
  tPrediction_->Branch("totalMuons",&totalMuons_);
  tPrediction_->Branch("elecAccWeight",&elecAccWeight_);
  tPrediction_->Branch("elecAccEff",&elecAccEff_);
  tPrediction_->Branch("elecRecoWeight",&elecRecoWeight_);
  tPrediction_->Branch("elecRecoEff",&elecRecoEff_);
  tPrediction_->Branch("elecIsoWeight",&elecIsoWeight_);
  tPrediction_->Branch("elecIsoOnlyWeight",&elecIsoOnlyWeight_);
  tPrediction_->Branch("elecIsoEff",&elecIsoEff_);
  tPrediction_->Branch("elecTotalWeight",&elecTotalWeight_);
  //  tPrediction_->Branch("expectationReductionIsoTrackEff",&expectationReductionIsoTrackEff_);
  tPrediction_->Branch("expectationReductionMuIsoTrackEff",&expectationReductionMuIsoTrackEff_);
  tPrediction_->Branch("expectationReductionElecIsoTrackEff",&expectationReductionElecIsoTrackEff_);
  tPrediction_->Branch("expectationReductionPionIsoTrackEff",&expectationReductionPionIsoTrackEff_);
  tPrediction_->Branch("expectationReductionIsoTrackEffCombined",&expectationReductionIsoTrackEffCombined_);
  tPrediction_->Branch("totalWeight",&totalWeight_);
  tPrediction_->Branch("muDiLepEffMTWAppliedEff",&muDiLepEffMTWAppliedEff_);
  tPrediction_->Branch("totalWeightDiLep",&totalWeightDiLep_);
  tPrediction_->Branch("isoMuonTracks",&isoMuonTracks);
  tPrediction_->Branch("isoElectronTracks",&isoElectronTracks);
  tPrediction_->Branch("isoPionTracks",&isoPionTracks);
  //  tPrediction_->Branch("totalStatUp", &totalStatUp);
  //  tPrediction_->Branch("totalStatDown", &totalStatDown);

  SearchBins_ = new SearchBins(true);

  if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntries();

  //  if ( max_evts > 0 && max_evts < nentries ) nentries = max_evts ;

  printf("\n\n Looping over %llu events.\n\n", nentries ) ;

  Long64_t nbytes = 0, nb = 0;

  for (Long64_t jentry=0; jentry<nentries;jentry++) {

    if ( jentry%(nentries/100) == 0 && jentry>0 ) { printf("  %9llu out of %9llu (%2lld%%)\n", jentry, nentries, ((100*jentry)/nentries)+1 ) ; fflush(stdout) ; }

    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;

    resetValues();
    
    if(HTgen_cut > 0.01 && genHT > HTgen_cut) continue;

    // basic kinematic cuts (probably applied at skim level)
    if(HT<500|| MHT< 200 || NJets < 7 || BTags < 2  ) continue;
    if(DeltaPhi1 < 0.5 || DeltaPhi2 < 0.5 || DeltaPhi3 < 0.3 || DeltaPhi4 < 0.3) continue;
    if(!FiltersPass() ) continue;
    //    cout << "Event passed all kinematic and cleaning cuts..." << endl;
    // if(useFilterList){
    //   bool CSCTightHaloFilterUpdate = evtListFilter->CheckEvent(RunNum,LumiBlockNum,EvtNum);
    //   if(!CSCTightHaloFilterUpdate) continue;
    // } 

    if (Muons->size()!=1 || Electrons->size()!=0) continue; // 1-muon control sample events
    // cout << "Single muon event..." << endl;


    // printf("MHT/HT/NJets/BTags: %3.2f/%3.2f/%d/%d\n", MHT, HT, NJets, BTags);
    // printf("Mu pt/eta/act: %3.2f/%3.2f/%f\n", Muons->at(0).Pt(), Muons->at(0).Eta(), selectedIDIsoMuons_MT2Activity->at(0));
    
    isoTracks= isoElectronTracks + isoMuonTracks + isoPionTracks;

    selectedIDMuonsNum_ = selectedIDMuons->size();
    selectedIDIsoMuonsNum_ = Muons->size();
    selectedIDElectronsNum_ = selectedIDElectrons->size();
    selectedIDIsoElectronsNum_ = Electrons->size();

    JetsNum_ = Jets->size();

    if(useTrigger) if(!TriggerPass->at(34) && !TriggerPass->at(35) && !TriggerPass->at(36)) return kTRUE;
    if(useTriggerEffWeight) Weight *= GetTriggerEffWeight(MHT);

    // if(doPUreweighting){
    //   w_pu = puhist->GetBinContent(puhist->GetXaxis()->FindBin(min(TrueNumInteractions,puhist->GetBinLowEdge(puhist->GetNbinsX()+1))));
    //   Weight *= w_pu;
    // }

    if(runOnData) Weight = 1.;

    Bin_ = SearchBins_->GetBinNumber(HT,MHT,NJets,BTags);

    //  expectationReductionIsoTrackEffVec_= ExpectationReductionIsoTrackHTMHT_NJetsHighEff_->GetEff(HT, MHT, useAsymmErrors);
    expectationReductionMuIsoTrackEffVec_ = ExpectationReductionMuIsoTrackHTMHT_NJetsHighEff_->GetEff(HT, MHT, useAsymmErrors);
    expectationReductionElecIsoTrackEffVec_ = ExpectationReductionElecIsoTrackHTMHT_NJetsHighEff_->GetEff(HT, MHT, useAsymmErrors);
    expectationReductionPionIsoTrackEffVec_ = ExpectationReductionPionIsoTrackHTMHT_NJetsHighEff_->GetEff(HT, MHT, useAsymmErrors);

    //for compatibility
    // expectationReductionIsoTrackEff_ = expectationReductionIsoTrackEffVec_.eff;
    expectationReductionMuIsoTrackEff_ = expectationReductionMuIsoTrackEffVec_.eff;
    expectationReductionElecIsoTrackEff_ = expectationReductionElecIsoTrackEffVec_.eff;
    expectationReductionPionIsoTrackEff_ = expectationReductionPionIsoTrackEffVec_.eff;


    // get Efficiencies
    muMTWEffVec_ = MuMTWHTNJets_->GetEff(HT, NJets, useAsymmErrors);
    muDiLepContributionMTWAppliedEffVec_ = MuDiLepContributionMTWAppliedNJets_->GetEff(NJets, false); 
    muDiLepEffMTWAppliedEffVec_ = MuDiLepEffMTWAppliedNJets_->GetEff(NJets, false);      // these were returning 0, so I turned off AsymmErrors
                                                                                        // seem to be symmetric anyway

    muIsoEffVec_ = MuIsoActivityPT_->GetEff(selectedIDIsoMuons_MT2Activity->at(0), Muons->at(0).Pt(), useAsymmErrors);
    muRecoEffVec_ = MuRecoActivityPT_->GetEff(selectedIDIsoMuons_MT2Activity->at(0), Muons->at(0).Pt(), useAsymmErrors);
    if(NJets<8.5) muAccEffVec_ = MuAccHTMHT_NJets78_->GetEff(HT,MHT, useAsymmErrors);
    else muAccEffVec_ = MuAccHTMHT_NJets9Inf_->GetEff(HT,MHT, useAsymmErrors);

    elecIsoEffVec_ = ElecIsoActivityPT_->GetEff(selectedIDIsoMuons_MT2Activity->at(0), Muons->at(0).Pt(), useAsymmErrors);
    elecRecoEffVec_ = ElecRecoActivityPT_->GetEff(selectedIDIsoMuons_MT2Activity->at(0), Muons->at(0).Pt(), useAsymmErrors);
    if(NJets<8.5) elecAccEffVec_ = ElecAccHTMHT_NJets78_->GetEff(HT,MHT, useAsymmErrors);
    else elecAccEffVec_ = ElecAccHTMHT_NJets9Inf_->GetEff(HT,MHT, useAsymmErrors);
	

    //for compatibility
    muMTWEff_ = muMTWEffVec_.eff;
    muDiLepContributionMTWAppliedEff_ = muDiLepContributionMTWAppliedEffVec_.eff;
    muDiLepEffMTWAppliedEff_ = muDiLepEffMTWAppliedEffVec_.eff;
    muIsoEff_ = muIsoEffVec_.eff;
    muRecoEff_ = muRecoEffVec_.eff;
    muAccEff_ = muAccEffVec_.eff;
    elecAccEff_ = elecAccEffVec_.eff;
    elecRecoEff_ = elecRecoEffVec_.eff;
    elecIsoEff_ = elecIsoEffVec_.eff;      

    // calculate Weights
    // muCS >99% purity
    //purityCorrectedWeight_ = Weight * muPurityCorrection_;
    purityCorrectedWeight_ = Weight;
    mtwCorrectedWeight_ = purityCorrectedWeight_ / muMTWEff_;  
    mtwDiLepCorrectedWeight_ = mtwCorrectedWeight_ * muDiLepContributionMTWAppliedEff_;

    //    printf("purityCorrectedWeight / mtwCorrectedWeight / mtwDiLepCorrectedWeight = %f / %f / %f\n", purityCorrectedWeight_, mtwCorrectedWeight_, mtwDiLepCorrectedWeight_);

    //    printf("muIsoEff / muRecoEff / muAccEff = %f / %f / %f\n", muIsoEff_, muRecoEff_, muAccEff_);
    muIsoWeight_ = mtwDiLepCorrectedWeight_* (1 - muIsoEff_)/muIsoEff_; 
    muRecoWeight_ = mtwDiLepCorrectedWeight_* 1 / muIsoEff_ * (1-muRecoEff_)/muRecoEff_;    
    muAccWeight_ = mtwDiLepCorrectedWeight_* 1 / muIsoEff_ * 1 / muRecoEff_ * (1-muAccEff_)/muAccEff_;    
    //    printf("muIsoWeight / muRecoWeight / muAccWeight = %f / %f / %f\n", muIsoWeight_, muRecoWeight_, muAccWeight_);
    muTotalWeight_ = muIsoWeight_ + muRecoWeight_ + muAccWeight_;
    totalMuons_ = mtwDiLepCorrectedWeight_ / ( muIsoEff_ * muRecoEff_ * muAccEff_);    
    //    printf("muTotalWeight / totalMuons = %f / %f\n", muTotalWeight_, totalMuons_);

    //    printf("elecIsoEff / elecRecoEff / elecAccEff = %f / %f / %f\n", elecIsoEff_, elecRecoEff_, elecAccEff_);
    elecAccWeight_ = totalMuons_ * (1 - elecAccEff_);
    elecRecoWeight_ = totalMuons_ * (elecAccEff_) * (1-elecRecoEff_);
    elecIsoWeight_ = totalMuons_ * (elecAccEff_) * (elecRecoEff_) * (1-elecIsoEff_);
    //    printf("elecIsoWeight / elecRecoWeight / elecAccWeight = %f / %f / %f\n", elecIsoWeight_, elecRecoWeight_, elecAccWeight_);
    elecTotalWeight_ = elecIsoWeight_ + elecRecoWeight_ + elecAccWeight_;
    totalElectrons_ = mtwDiLepCorrectedWeight_ / ( elecIsoEff_ * elecRecoEff_ * elecAccEff_);
    //    printf("elecTotalWeight / totalElectrons = %f / %f\n", elecTotalWeight_, totalElectrons_);
    totalWeight_ = elecTotalWeight_ + muTotalWeight_;

    totalWeightDiLep_ = totalWeight_ + (1-muDiLepContributionMTWAppliedEff_) * mtwCorrectedWeight_ * (1-muDiLepEffMTWAppliedEff_)/muDiLepEffMTWAppliedEff_;
    totalWeightDiLepIsoTrackReducedCombined_ = totalWeightDiLep_ * (1 - (expectationReductionMuIsoTrackEff_ + expectationReductionElecIsoTrackEff_ + expectationReductionPionIsoTrackEff_));
    //    printf("totalWeight / totalWeightDiLep / totalWeightDiLepIsoTrackReducedCombined = %f / %f / %f\n", totalWeight_, totalWeightDiLep_, totalWeightDiLepIsoTrackReducedCombined_);

    MTW=selectedIDIsoMuons_MTW->at(0);
    
    if(MTW<100){
      MuMeanWeight_->Fill(Bin_+0.01, totalWeightDiLepIsoTrackReduced_/Weight, Weight);
      if(Bin_<=6){
	MuWeightPerBin_[Bin_-1]->Fill(totalWeightDiLepIsoTrackReduced_/Weight,Weight);
      }
    }

    //    cout << "Write event to tree..." << endl;
    tPrediction_->Fill();

    
  } // end event loop

  outPutFile->cd();
  tPrediction_->Write();
  outPutFile->mkdir("MeanWeight");
  TDirectory *dEfficiencies = (TDirectory*)outPutFile->Get("MeanWeight");
  dEfficiencies->cd();
  MuMeanWeight_->Write();

  outPutFile->cd();
  outPutFile->mkdir("WeightPerBin");
  TDirectory *dEfficienciesPerBin = (TDirectory*)outPutFile->Get("WeightPerBin");
  dEfficienciesPerBin->cd();
  for(int b = 0; b <6; b++){
    MuWeightPerBin_[b]->Write();
  }

  outPutFile->Close();

  cout << "Saved output to " << outputFileName << endl;
  
}

void PredictionMaker::resetValues()
{
  MTW=0.;
  muMTWEff_=0.;
  mtwCorrectedWeight_=0.;
  muDiLepContributionMTWAppliedEff_=0.;
  mtwDiLepCorrectedWeight_=0.;
  muIsoEff_=0;
  muIsoWeight_=0;
  muRecoEff_=0;
  muRecoWeight_=0;
  muAccEff_=0;
  muAccWeight_=0;
  muTotalWeight_=0.;
  totalMuons_=0;
  elecIsoEff_=0;
  elecIsoWeight_=0;
  elecRecoEff_=0;
  elecRecoWeight_=0;
  elecAccEff_=0;
  elecAccWeight_=0;
  elecTotalWeight_=0;
  totalElectrons_=0.;
  totalWeight_=0.;
  muDiLepEffMTWAppliedEff_=0.;
  totalWeightDiLep_=0.;
  totalWeightDiLepIsoTrackReduced_=0.;
}

bool PredictionMaker::FiltersPass()
{
  bool result=true;
  //if(useFilterData){
    //if(!CSCTightHaloFilter) result=false;
    if(eeBadScFilter!=1) result=false;
    if(!eeBadSc4Filter) result=false;
    if(!HBHENoiseFilter) result=false;
    if(!HBHEIsoNoiseFilter) result=false;
    //  }
  if(NVtx<=0) result=false;
  // Do not apply on fastSim samples!
  // if(!signalScan) if(JetID!=1) result=false;
  return result;
}
