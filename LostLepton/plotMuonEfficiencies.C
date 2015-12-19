// Plot the muon acceptance and reconstruction and isolation efficiencies
//
// Author: Matthias Schroeder
//         matthias.schroeder@AT@desy.de
//         November 2013

void plotMuonEfficiencies(const TString &fileName = "../data/LostLepton_MuonEfficienciesFromWJetMC.root") {
  gROOT->ProcessLine(".L ../Utils/Sample.h+");
  gROOT->ProcessLine(".L ../Utils/StyleMatters.h+");
  gROOT->ProcessLine(".L ../Utils/HistReader.h+");
  gROOT->ProcessLine(".L ../Utils/LeptonEfficiency.h+");
  gROOT->ProcessLine(".L ../Utils/LeptonAcceptance.h+");
  StyleMatters::init();

  TH2* hAcc = static_cast<TH2*>(HistReader::get(fileName,LeptonAcceptance::nameMuonAcc()));
  hAcc->SetTitle("#mu acceptance");
  TCanvas* canAcc = new TCanvas("canAcc","Acc 2D",500,500);
  canAcc->cd();
  StyleMatters::adaptForColZ(canAcc);
  hAcc->Draw("colz");
  canAcc->SaveAs("acc.png");

  for(unsigned int i = 0; i < LeptonEfficiency::numNJetBins(); ++i) {
    TH2* hRecoEff = static_cast<TH2*>(HistReader::get(fileName,LeptonEfficiency::nameMuonRecoEff(i)));
    hRecoEff->SetTitle("#mu reco eff, "+LeptonEfficiency::nJetBinId(i));
    TCanvas* canRecoEff = new TCanvas("canRecoEff"+LeptonEfficiency::nJetBinId(i),"Reco Eff "+LeptonEfficiency::nJetBinId(i),500,500);
    canRecoEff->cd();
    StyleMatters::adaptForColZ(canRecoEff);
    hRecoEff->Draw("colz");
    canRecoEff->SaveAs("reco_"+LeptonEfficiency::nJetBinId(i)+".png");
    
    TH2* hIsoEff = static_cast<TH2*>(HistReader::get(fileName,LeptonEfficiency::nameMuonIsoEff(i)));
    hIsoEff->SetTitle("#mu iso eff, "+LeptonEfficiency::nJetBinId(i));
    TCanvas* canIsoEff = new TCanvas("canIsoEff"+LeptonEfficiency::nJetBinId(i),"Iso Eff "+LeptonEfficiency::nJetBinId(i),500,500);
    canIsoEff->cd();
    StyleMatters::adaptForColZ(canIsoEff);
    hIsoEff->Draw("colz");
    canRecoEff->SaveAs("iso_"+LeptonEfficiency::nJetBinId(i)+".png");
  }
}
