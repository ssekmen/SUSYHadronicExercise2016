// Plot the lost-lepton prediction and print the event yields
//
// Author: Matthias Schroeder
//         matthias.schroeder@AT@desy.de
//         November 2013
void plotPrediction(double scale = 1., const TString &fileName = "LostLepton_Data_Prediction.root") {
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
  sprintf(entry,"N(total) = %.0f",hMuonPt->Integral());
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

  // The predictions
  for(unsigned int i = 0; i < 3; ++i) {
    TH1* hPred = NULL;
    if( i == 0 ) {
      hPred = HistReader::get(fileName,"hHtPred");
    } else if( i == 1 ) {
      hPred = HistReader::get(fileName,"hMhtPred");
    } else if( i == 2 ) {
      hPred = HistReader::get(fileName,"hNJetsPred");
    }
    hPred->SetLineWidth(2);
    hPred->SetMarkerStyle(20);
    hPred->Scale(scale);

    TString name = "can";
    name += hPred->GetName();
    TCanvas* can = new TCanvas(name,name,500,500);
    can->cd();
    hPred->Draw("PE1");
    can->SetLogy();
  }


  // Print event yields
  std::cout << "Number of control-sample events:" << hMuonPt->Integral(1,100000) << std::endl;;

  TH1* hYieldsPred = HistReader::get(fileName,"hYieldsPred");
  hYieldsPred->Scale(scale);
  std::cout << "Event yields" << std::endl;
  std::cout << "Selection : \tPredicted Yields\t\t" << std::endl;
  for(int bin = 1; bin <= hYieldsPred->GetNbinsX(); ++bin) {
    std::cout << hYieldsPred->GetXaxis()->GetBinLabel(bin) << " : \t" << std::flush;
    std::cout << hYieldsPred->GetBinContent(bin) << " +/- " << hYieldsPred->GetBinError(bin) << "  \t" << std::flush;
    std::cout << std::endl;
  }
}
