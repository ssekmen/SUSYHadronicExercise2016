// Compare the sum of the HT, MHT, and N(jets) distributions
// for the background samples to the observed distributions in
// data.
//
// Author: Matthias Schroeder
//         matthias.schroeder@AT@desy.de
//         November 2013


// === Helper Functions ================================================

// Set the histogram style according to the sample id
void setStyle(TH1* h, unsigned int id) {
  h->SetLineWidth(2);
  h->SetLineColor(Sample::color(id));
  h->SetMarkerColor(Sample::color(id));
  if( id < 10 ) {		// This is a data sample
    h->SetMarkerStyle(20);
  } else {
    h->SetMarkerStyle(1);
    h->SetFillColor(Sample::color(id));
  }
}
    


// === Main Function ===================================================
void plotDataVsMC(const TString &graphicsFormat = "png") {
  gROOT->ProcessLine(".L ../Utils/Sample.h+");
  gROOT->ProcessLine(".L ../Utils/StyleMatters.h+");
  gROOT->ProcessLine(".L ../Utils/HistReader.h+");
  StyleMatters::init();
  const int canSize = 500;

  // Get the data distributions
  const TString dataFileName = "General_"+Sample::toTString(1)+"-Yields.root";
  TH1* hDataNJets = HistReader::get(dataFileName,"hNJets");
  TH1* hDataHt = HistReader::get(dataFileName,"hHt");
  TH1* hDataMht = HistReader::get(dataFileName,"hMht");
  TH1* hDataYields = HistReader::get(dataFileName,"hYields");
  // Set style and color
  setStyle(hDataNJets,1);
  setStyle(hDataHt,1);
  setStyle(hDataMht,1);
  setStyle(hDataYields,1);

  // Get the bkg distributions
  const int kNBkgSamples = 4;
  unsigned int ids[kNBkgSamples] = { 14, 12, 13, 11 };
  THStack* hBkgNJets = new THStack("hBkgNJets",";N(jets);Events");
  THStack* hBkgHt = new THStack("hBkgHt",";H_{T} [GeV];Events");
  THStack* hBkgMht = new THStack("hBkgMht",";#slash{H}_{T} [GeV];Events");
  TLegend* leg = new TLegend(0.6,(0.89-0.05*(kNBkgSamples+1)),0.9,0.89);
  leg->SetBorderSize(0);
  leg->SetFillColor(0);
  leg->SetFillStyle(0);
  leg->SetTextFont(42);
  leg->AddEntry(hDataHt,Sample::label(1),"P");
  TH1* hBkgYields[kNBkgSamples];
  for(int s = 0; s < kNBkgSamples; ++s) { // Loop over samples
    // Get histograms from file
    const TString fileName = "General_"+Sample::toTString(ids[s])+"-Yields.root";
    TH1* hNJets = HistReader::get(fileName,"hNJets");
    TH1* hHt = HistReader::get(fileName,"hHt");
    TH1* hMht = HistReader::get(fileName,"hMht");
    hBkgYields[s] = HistReader::get(fileName,"hYields");
    // Set style and color
    setStyle(hNJets,ids[s]);
    setStyle(hHt,ids[s]);
    setStyle(hMht,ids[s]);
    setStyle(hBkgYields[s],ids[s]);
    // Add legend entry
    leg->AddEntry(hHt,Sample::label(ids[s]),"F");
    // Add histogram to stack
    hBkgNJets->Add(hNJets);
    hBkgHt->Add(hHt);
    hBkgMht->Add(hMht);
  } // End of loop over samples

  // Draw
  TCanvas* canNJets = new TCanvas("canNJets","NJets",canSize,canSize);
  canNJets->cd();
  hBkgNJets->Draw("HIST");
  hDataNJets->Draw("PE1same");
  leg->Draw("same");
  canNJets->SetLogy();
  gPad->RedrawAxis();
  canNJets->SaveAs("hDataVsMC_NJets."+graphicsFormat);

  TCanvas* canHt = new TCanvas("canHt","Ht",canSize,canSize);
  canHt->cd();
  hBkgHt->Draw("HIST");
  hDataHt->Draw("PE1same");
  leg->Draw("same");
  canHt->SetLogy();
  gPad->RedrawAxis();
  canHt->SaveAs("hDataVsMC_Ht."+graphicsFormat);

  TCanvas* canMht = new TCanvas("canMht","Mht",canSize,canSize);
  canMht->cd();
  hBkgMht->Draw("HIST");
  hDataMht->Draw("PE1same");
  leg->Draw("same");
  canMht->SetLogy();
  gPad->RedrawAxis();
  canMht->SaveAs("hDataVsMC_Mht."+graphicsFormat);

  // Print event yields
  std::cout << "Event yields" << std::endl;
  std::cout << "Selection\tData\t\t\t" << std::flush;
  for(int s = 0; s < kNBkgSamples; ++s) {
    std::cout << Sample::toTString(ids[s]) << " \t\t\t" << std::flush;
  }
  std::cout << std::endl;
  for(int bin = 1; bin <= hDataYields->GetNbinsX(); ++bin) {
    std::cout << hDataYields->GetXaxis()->GetBinLabel(bin) << " : \t" << std::flush;
    std::cout << hDataYields->GetBinContent(bin) << " +/- " << std::flush;
    std::cout << hDataYields->GetBinError(bin) << "  \t" << std::flush;
    for(int s = 0; s < kNBkgSamples; ++s) {
      std::cout << hBkgYields[s]->GetBinContent(bin) << " +/- " << std::flush;
      std::cout << hBkgYields[s]->GetBinError(bin) << "  \t" << std::flush;
    }
    std::cout << std::endl;
  }
}
