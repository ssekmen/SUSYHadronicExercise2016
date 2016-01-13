// Compare the sum of the HT, MHT, and N(jets) distributions
// for the background samples to the observed distributions in
// data.
//
// Author: Matthias Schroeder
//         matthias.schroeder@AT@desy.de
//         November 2013

#include "../Utils/Sample.h"
#include "../Utils/StyleMatters.h"
#include "../Utils/HistReader.h"

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
  THStack* hBkgYieldsStack = new THStack("hBkgYieldsStack",";Yields at baseline and search bins");
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
    TH1* hYields = HistReader::get(fileName,"hYields");
    hBkgYields[s] = HistReader::get(fileName,"hYields");
    // Set style and color
    setStyle(hNJets,ids[s]);
    setStyle(hHt,ids[s]);
    setStyle(hMht,ids[s]);
    setStyle(hYields,ids[s]);
    // Add legend entry
    leg->AddEntry(hHt,Sample::label(ids[s]),"F");
    // Add histogram to stack
    hBkgNJets->Add(hNJets);
    hBkgHt->Add(hHt);
    hBkgMht->Add(hMht);
    hBkgYieldsStack->Add(hYields);
  } // End of loop over samples

  // Draw
  TCanvas* canNJets = new TCanvas("canNJets","NJets",canSize,canSize);
  canNJets->cd();
  hBkgNJets->SetMaximum(10000.);
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

  TCanvas* canYields = new TCanvas("canYields","Yields",canSize,canSize);
  canYields->cd();
  hBkgYieldsStack->Draw("HIST");
  hDataYields->Draw("PE1same");
  leg->Draw("same");
  canYields->SetLogy();
  gPad->RedrawAxis();
  canYields->SaveAs("hDataVsMC_Yields."+graphicsFormat);

  // Print event yields
  std::cout << "Event yields" << std::endl;
  std::cout << "Selection\t" << std::flush;
  std::string str = Sample::toTString(1).Data();
  printf(" %-20s:",str.c_str());
  for(int s = 0; s < kNBkgSamples; ++s) {
    str = Sample::toTString(ids[s]).Data();
    printf(" %-20s:",str.c_str());
  }
  printf(" Total               :");
  std::cout << std::endl;
  double BkgTotal[6]={0.,0.,0.,0.,0.,0.};
  double BkgTotalError[6]={0.,0.,0.,0.,0.,0.};
  for(int bin = 1; bin <= hDataYields->GetNbinsX(); ++bin) {
    std::cout << hDataYields->GetXaxis()->GetBinLabel(bin) << " : \t" << std::flush;
    printf("%8.1f +/- ",hDataYields->GetBinContent(bin));
    printf("%7.3f :",hDataYields->GetBinError(bin));
    for(int s = 0; s < kNBkgSamples; ++s) {
      printf("%8.3f +/- ",hBkgYields[s]->GetBinContent(bin));
      printf("%7.3f :",hBkgYields[s]->GetBinError(bin));
      BkgTotal[bin-1]      += hBkgYields[s]->GetBinContent(bin);
      BkgTotalError[bin-1] += pow(hBkgYields[s]->GetBinError(bin),2);
    }
    printf("%8.3f +/- ",BkgTotal[bin-1]);
    printf("%7.3f :",sqrt(BkgTotalError[bin-1]));
    std::cout << std::endl;
  }
}
