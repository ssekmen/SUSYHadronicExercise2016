#define EfficiencyMaker_cxx
#include "EfficiencyMaker.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void EfficiencyMaker::Run(std::string outputFileName)
{

  TH1::SetDefaultSumw2();
  gSystem->mkdir("Efficiencies"); 

  const int muaccHT_ = 4;
  double muAccHT_ [muaccHT_] = {500,800,1200,10000};
  const int muaccMHT2D_ = 3;
  double muAccMHT2D_ [muaccMHT2D_] = {200,500,1900};
 
  MuAccHTMHT_NJets78_ = new TH2Eff("MuAccHTMHT_NJets78","MuAccHTMHT_NJets78",muaccHT_-1,muAccHT_, muaccMHT2D_-1,muAccMHT2D_);
  MuAccHTMHT_NJets9Inf_ = new TH2Eff("MuAccHTMHT_NJets9Inf","MuAccHTMHT_NJets9Inf",muaccHT_-1,muAccHT_, muaccMHT2D_-1,muAccMHT2D_);

  const int elecaccHT_ = 4;
  double elecAccHT_ [elecaccHT_] = {500,800,1200,10000};
  const int elecaccMHT2D_ = 3;
  double elecAccMHT2D_ [elecaccMHT2D_] = {200,500,1900};
  ElecAccHTMHT_NJets78_ = new TH2Eff("ElecAccHTMHT_NJets78","ElecAccHTMHT_NJets78",elecaccHT_-1,elecAccHT_, elecaccMHT2D_-1,elecAccMHT2D_);
  ElecAccHTMHT_NJets9Inf_ = new TH2Eff("ElecAccHTMHT_NJets9Inf","ElecAccHTMHT_NJets9Inf",elecaccHT_-1,elecAccHT_, elecaccMHT2D_-1,elecAccMHT2D_);

  const int oneDPT_=8;
  double OneDPT_[oneDPT_]={10,15,20,30,40,50,100,2500};
  const int oneDActivity_=6;
  double OneDActivity_[oneDActivity_]={0, 0.02, 0.05, 0.2, 1., 50.};
  MuIsoActivityPT_ = new TH2Eff("MuIsoActivityPT","MuIsoActivityPT", oneDActivity_-1,OneDActivity_,oneDPT_-1,OneDPT_);
  ElecIsoActivityPT_ = new TH2Eff("ElecIsoActivityPT","ElecIsoActivityPT", oneDActivity_-1,OneDActivity_,oneDPT_-1,OneDPT_);
  MuRecoActivityPT_ = new TH2Eff("MuRecoActivityPT","MuRecoActivityPT", oneDActivity_-1,OneDActivity_,oneDPT_-1,OneDPT_);
  ElecRecoActivityPT_ = new TH2Eff("ElecRecoActivityPT","ElecRecoActivityPT", oneDActivity_-1,OneDActivity_,oneDPT_-1,OneDPT_);

  const int muaccNJets_ = 6;
  double muAccNJets_ [muaccNJets_] = {4,5,6,7,9,21};
  const int oneDNJets_=6;
  double OneDNJets_[oneDNJets_] = {4,5,6,7,9,21};
  MuMTWHTNJets_ = new TH2Eff("MuMTWHTNJets","MuMTWHTNJets",muaccHT_-1,muAccHT_, muaccNJets_-1, muAccNJets_);
  MuDiLepContributionMTWNJets_ = new TH1Eff("MuDiLepContributionMTWNJets1D","MuDiLepContributionMTWNJets1D",oneDNJets_-1,OneDNJets_);
  MuDiLepMTWNJets_ = new TH1Eff("MuDiLepMTWNJets1D","MuDiLepMTWNJets1D",oneDNJets_-1,OneDNJets_);

  const int isotrackreductionHT_ = 3;
  double isoTrackReductionHT_ [isotrackreductionHT_] = {500,1200,10000};
  const int isotrackreductionMHT_ = 3;
  double isoTrackReductionMHT_ [isotrackreductionMHT_] = {200,500,1900};
  MuIsoTrackReductionHTMHT_NJetsHigh_ = new TH2Eff("MuIsoTrackReductionHTMHT_NJetsHigh","MuIsoTrackReductionHTMHT_NJetsHigh", isotrackreductionHT_-1,isoTrackReductionHT_, isotrackreductionMHT_-1,isoTrackReductionMHT_);
  ElecIsoTrackReductionHTMHT_NJetsHigh_ = new TH2Eff("ElecIsoTrackReductionHTMHT_NJetsHigh","ElecIsoTrackReductionHTMHT_NJetsHigh", isotrackreductionHT_-1,isoTrackReductionHT_, isotrackreductionMHT_-1,isoTrackReductionMHT_);
  PionIsoTrackReductionHTMHT_NJetsHigh_ = new TH2Eff("PionIsoTrackReductionHTMHT_NJetsHigh","PionIsoTrackReductionHTMHT_NJetsHigh", isotrackreductionHT_-1,isoTrackReductionHT_, isotrackreductionMHT_-1,isoTrackReductionMHT_);


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

    if(Weight<0) continue;
    if(Bin > 900) continue;

    // calculate muon efficiencies
    // acceptance
    if(muAcc==2) {
      if(NJets > 6.5 && NJets < 8.5) MuAccHTMHT_NJets78_->Fill(HT,MHT,Weight,true);
      if(NJets > 8.5) MuAccHTMHT_NJets9Inf_->Fill(HT,MHT,Weight,true);
    } else if (muAcc==0) {
      if(NJets > 6.5 && NJets < 8.5) MuAccHTMHT_NJets78_->Fill(HT,MHT,Weight,false);
      if(NJets > 8.5) MuAccHTMHT_NJets9Inf_->Fill(HT,MHT,Weight,false);
    }
    // reconstruction
    if(muReco==2) MuRecoActivityPT_->Fill(GenMu_MT2Activity->at(0),GenMus->at(0).Pt(),Weight,true);
    else if(muReco==0)  MuRecoActivityPT_->Fill(GenMu_MT2Activity->at(0),GenMus->at(0).Pt(),Weight,false);
    // isolation
    if(muIso==2) MuIsoActivityPT_->Fill(GenMu_MT2Activity->at(0),GenMus->at(0).Pt(),Weight,true);
    else if(muIso==0)  MuIsoActivityPT_->Fill(GenMu_MT2Activity->at(0),GenMus->at(0).Pt(),Weight,false);


    // calculate electron efficiencies
    // acceptance
    if(elecAcc==2)  {
      if(NJets > 6.5 && NJets < 8.5) ElecAccHTMHT_NJets78_->Fill(HT,MHT,Weight,true);
      if(NJets > 8.5) ElecAccHTMHT_NJets9Inf_->Fill(HT,MHT,Weight,true);
    } else if (elecAcc==0) {
      if(NJets > 6.5 && NJets < 8.5) ElecAccHTMHT_NJets78_->Fill(HT,MHT,Weight,false);
      if(NJets > 8.5) ElecAccHTMHT_NJets9Inf_->Fill(HT,MHT,Weight,false);
    }
    // reconstruction
    if(elecReco==2) ElecRecoActivityPT_->Fill(GenElec_MT2Activity->at(0),GenEls->at(0).Pt(),Weight,true);
    else if(elecReco==0)  ElecRecoActivityPT_->Fill(GenElec_MT2Activity->at(0),GenEls->at(0).Pt(),Weight,false);
    // isolation
    if(elecIso==2) ElecIsoActivityPT_->Fill(GenElec_MT2Activity->at(0),GenEls->at(0).Pt(),Weight,true);
    else if(elecIso==0)  ElecIsoActivityPT_->Fill(GenElec_MT2Activity->at(0),GenEls->at(0).Pt(),Weight,false);

    // mt cut, dilepton contribution (just needed for muons)
    if(muIso==2 && MTW < 100) MuMTWHTNJets_->Fill(HT, NJets, Weight, true);
    else if(muIso==2 && MTW > 100) MuMTWHTNJets_->Fill(HT, NJets, Weight, false);

    if (MuDiLepControlSample==2) {
      MuDiLepMTWNJets_->Fill(NJets,Weight,true);
      MuDiLepContributionMTWNJets_->Fill(NJets,Weight,true);
    } else if(MuDiLepControlSample==0) {
      MuDiLepMTWNJets_->Fill(NJets,Weight,false);
      MuDiLepContributionMTWNJets_->Fill(NJets,Weight,false);
    }

    // track veto
    if(Expectation==1) {
      if(isoMuonTracks>0) MuIsoTrackReductionHTMHT_NJetsHigh_->Fill(HT,MHT,Weight,true);
      else MuIsoTrackReductionHTMHT_NJetsHigh_->Fill(HT,MHT,Weight,false);
      if(isoElectronTracks>0) ElecIsoTrackReductionHTMHT_NJetsHigh_->Fill(HT,MHT,Weight,true);
      else ElecIsoTrackReductionHTMHT_NJetsHigh_->Fill(HT,MHT,Weight,false);
      if(isoPionTracks>0) PionIsoTrackReductionHTMHT_NJetsHigh_->Fill(HT,MHT,Weight,true);
      else PionIsoTrackReductionHTMHT_NJetsHigh_->Fill(HT,MHT,Weight,false);
    }
       
  } // end event loop

    // save efficiencies to histograms
  TFile *outPutFile = new TFile(outputFileName.c_str(),"RECREATE"); 
  outPutFile->cd();
  outPutFile->mkdir("Efficiencies");
  TDirectory *dEfficiencies = (TDirectory*)outPutFile->Get("Efficiencies");
  dEfficiencies->cd();
  gStyle->SetPaintTextFormat("5.2f");
  gStyle->SetStatW(0.1);
  gStyle->SetStatH(0.1);
  gStyle->SetStatY(202);
  gStyle->SetTitleYOffset(1.3);

  gStyle->SetPalette(56);
  gStyle->SetMarkerSize(1.3);

  MuAccHTMHT_NJets78_->SaveEff("#mu acc N_{Jets}=7-8;H_{T} [GeV]; #slash{H}_{T} [GeV]", dEfficiencies);   
  MuAccHTMHT_NJets9Inf_->SaveEff("#mu acc N_{Jets}=9+;H_{T} [GeV]; #slash{H}_{T} [GeV]", dEfficiencies);   
  ElecAccHTMHT_NJets78_->SaveEff("e acc N_{Jets}=7-8;H_{T} [GeV]; #slash{H}_{T} [GeV]", dEfficiencies);   
  ElecAccHTMHT_NJets9Inf_->SaveEff("e acc N_{Jets}=9+;H_{T} [GeV]; #slash{H}_{T} [GeV]", dEfficiencies);   
  MuRecoActivityPT_->SaveEff("#mu reco; Activity; p_{T}(#mu) [GeV]", dEfficiencies, true);   
  MuIsoActivityPT_->SaveEff("#mu Iso; Activity; p_{T}(#mu) [GeV]", dEfficiencies, true);   
  ElecRecoActivityPT_->SaveEff("e reco; Activity; p_{T}(e) [GeV]", dEfficiencies, true);   
  ElecIsoActivityPT_->SaveEff("e Iso; Activity; p_{T}(e) [GeV]", dEfficiencies, true);   

  MuMTWHTNJets_->SaveEff("#mu m_{T}(w); H_{T} [GeV]; N_{Jets}", dEfficiencies);
  MuDiLepMTWNJets_->SaveEff("#mu di lep; N_{Jets}", dEfficiencies);   
  MuDiLepContributionMTWNJets_->SaveEff("#mu di lep contri contri; N_{Jets}", dEfficiencies);   

  MuIsoTrackReductionHTMHT_NJetsHigh_->SaveEff("#mu iso track expec. reduction N_{Jets}#geq7; H_{T} [GeV]; #slash{H}_{T} [GeV]", dEfficiencies); 
  ElecIsoTrackReductionHTMHT_NJetsHigh_->SaveEff("e iso track expec. reduction N_{Jets}#geq7; H_{T} [GeV]; #slash{H}_{T} [GeV]", dEfficiencies);   
  PionIsoTrackReductionHTMHT_NJetsHigh_->SaveEff("#pi iso track expec. reduction N_{Jets}#geq7; H_{T} [GeV]; #slash{H}_{T} [GeV]", dEfficiencies); 

  outPutFile->Close();
      
}
