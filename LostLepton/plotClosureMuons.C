// Plot closure test for the lost-muon prediction
//
// Author: Matthias Schroeder
//         matthias.schroeder@AT@desy.de
//         November 2013

void plotClosureMuons(const TString &fileName = "LostLepton_ClosureMuonAcceptance.root") {
  gROOT->ProcessLine(".L ../Utils/Sample.h+");
  gROOT->ProcessLine(".L ../Utils/StyleMatters.h+");
  gROOT->ProcessLine(".L ../Utils/HistReader.h+");
  StyleMatters::init();

  // The muon kinematics
  TH1* hMuonPt = HistReader::get(fileName,"hMuonPt");
  TH1* hMuonEta = HistReader::get(fileName,"hMuonEta");
  hMuonPt->SetLineWidth(2);
  hMuonEta->SetLineWidth(2);
  hMuonPt->SetMarkerStyle(20);
  hMuonEta->SetMarkerStyle(20);

  TLegend* leg = new TLegend(0.4,0.8,0.9,0.89);
  leg->SetBorderSize(0);
  leg->SetFillColor(0);
  leg->SetFillStyle(0);
  leg->SetTextFont(42);
  leg->SetTextSize(0.05);
  char entry[100];
  sprintf(entry,"N(total) = %.1f",hMuonPt->Integral());
  leg->AddEntry(hMuonPt,entry,"");

  TCanvas* canMuPt = new TCanvas("canMuPt","Muon Pt",500,500);
  canMuPt->cd();
  hMuonPt->Draw("PE1");
  leg->Draw("same");
  canMuPt->SetLogy();
  TCanvas* canMuEta = new TCanvas("canMuEta","Muon Eta",500,500);
  canMuEta->cd();
  hMuonEta->Draw("PE1");
  leg->Draw("same");

  // Prediction vs truth
  for(unsigned int i = 0; i < 3; ++i) {
    TH1* hTrue = NULL;
    TH1* hPred = NULL;
    if( i == 0 ) {
      hTrue = HistReader::get(fileName,"hHt");
      hPred = HistReader::get(fileName,"hHtPred");
    } else if( i == 1 ) {
      hTrue = HistReader::get(fileName,"hMht");
      hPred = HistReader::get(fileName,"hMhtPred");
    } else if( i == 2 ) {
      hTrue = HistReader::get(fileName,"hNJets");
      hPred = HistReader::get(fileName,"hNJetsPred");
    }
    hTrue->SetLineWidth(2);
    hPred->SetLineWidth(2);
    hPred->SetMarkerStyle(20);

    TLegend* leg = new TLegend(0.6,0.75,0.9,0.87);
    leg->SetBorderSize(0);
    leg->SetTextFont(42);
    leg->SetFillColor(0);
    leg->AddEntry(hTrue,"Expectation","L");
    leg->AddEntry(hPred,"Prediction","P");

    TString name = "can";
    name += hTrue->GetName();
    TCanvas* can = new TCanvas(name,name,500,500);
    can->cd();
    hTrue->Draw("HISTE");
    hPred->Draw("PE1same");
    leg->Draw("same");
    can->SetLogy();

    name = hTrue->GetName();
    name += "Ratio";
    TH1* hRatio = static_cast<TH1*>(hPred->Clone(name));
    hRatio->Divide(hTrue);
    hRatio->GetYaxis()->SetTitle("N(predicted) / N(expected)");
    hRatio->GetYaxis()->SetRangeUser(0,2);

    TH1* hRatioFrame = static_cast<TH1*>(hRatio->Clone(name+"Frame"));
    for(int bin = 1; bin <= hRatioFrame->GetNbinsX(); ++bin) {
      hRatioFrame->SetBinContent(bin,1.);
    }
    hRatioFrame->SetLineStyle(2);
    hRatioFrame->SetLineWidth(2);
    hRatioFrame->SetLineColor(hTrue->GetLineColor());

    TString name = "can";
    name += hTrue->GetName();
    name += "Ratio";
    can = new TCanvas(name,name,500,500);
    can->cd();
    hRatioFrame->Draw("HIST");
    hRatio->Draw("PE1same");
  }

  // Print event yields
  TH1* hYieldsPred = HistReader::get(fileName,"hYieldsPred");
  TH1* hYieldsTrue = HistReader::get(fileName,"hYields");
  const bool isClosure = fileName.Contains("Closure");

  std::cout << "Event yields" << std::endl;
  std::cout << "Selection : \tPredicted Yields\t\t" << std::flush;
  if( isClosure ) std::cout << "True Yields" << std::flush;
  std::cout << std::endl;
  for(int bin = 1; bin <= hYieldsPred->GetNbinsX(); ++bin) {
    std::cout << hYieldsPred->GetXaxis()->GetBinLabel(bin) << " : \t" << std::flush;
    std::cout << hYieldsPred->GetBinContent(bin) << " +/- " << hYieldsPred->GetBinError(bin) << "  \t" << std::flush;
    if( isClosure ) std::cout << hYieldsTrue->GetBinContent(bin) << " +/- " << hYieldsTrue->GetBinError(bin) << "  \t" << std::flush;
    std::cout << std::endl;
  }

}
