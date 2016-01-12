// Check the sum of the GenHT, HT, MHT, and N(jets) distributions
// for the background samples in the W+Jets HTbinned samples
//
// Author: Kenichi Hatakeyama
//         Kenichi_Hatakeyama@baylor.edu
//         January 2016

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
void plot_WJets_HTbinned(const TString &graphicsFormat = "png") {

  StyleMatters::init();
  const int canSize = 500;

  // Get the bkg distributions
  const int kNSamples = 7;
  unsigned int ids[kNSamples] = { 111, 112, 113, 114, 115, 116, 117 };
  THStack* hStackNJets = new THStack("hNJets",";N(jets);Events");
  THStack* hStackHt    = new THStack("hHt",";H_{T} [GeV];Events");
  THStack* hStackMht   = new THStack("hMht",";#slash{H}_{T} [GeV];Events");
  THStack* hStackGenHt = new THStack("hGenHt",";Generator-level H_{T} [GeV];Events");
  TLegend* leg = new TLegend(0.5,(0.89-0.05*(kNSamples+1)),0.9,0.89);
  leg->SetBorderSize(0);
  leg->SetFillColor(0);
  leg->SetFillStyle(0);
  leg->SetTextFont(42);
  for(int s = 0; s < kNSamples; ++s) { // Loop over samples
    // Get histograms from file
    const TString fileName = "General_"+Sample::toTString(ids[s])+".root";
    TH1* hNJets = HistReader::get(fileName,"hNJets");
    TH1* hHt = HistReader::get(fileName,"hHt");
    TH1* hMht = HistReader::get(fileName,"hMht");
    TH1* hGenHt = HistReader::get(fileName,"hGenHt");
    // Set style and color
    setStyle(hNJets,ids[s]);
    setStyle(hHt,ids[s]);
    setStyle(hMht,ids[s]);
    setStyle(hGenHt,ids[s]);
    // Add legend entry
    leg->AddEntry(hHt,Sample::label(ids[s]),"F");
    // Add histogram to stack
    hStackNJets->Add(hNJets);
    hStackHt->Add(hHt);
    hStackMht->Add(hMht);
    hStackGenHt->Add(hGenHt);
  } // End of loop over samples

  // Draw
  TCanvas* canGenHt = new TCanvas("canGenHt","GenHt",canSize,canSize);
  canGenHt->cd();
  hStackGenHt->Draw("HIST");
  leg->Draw("same");
  canGenHt->SetLogy();
  gPad->RedrawAxis();
  canGenHt->SaveAs("hWJets_HTbinned_GenHt."+graphicsFormat);

  TCanvas* canNJets = new TCanvas("canNJets","NJets",canSize,canSize);
  canNJets->cd();
  hStackNJets->SetMaximum(100000.);
  hStackNJets->Draw("HIST");
  leg->Draw("same");
  canNJets->SetLogy();
  gPad->RedrawAxis();
  canNJets->SaveAs("hWJets_HTbinned_NJets."+graphicsFormat);

  TCanvas* canHt = new TCanvas("canHt","Ht",canSize,canSize);
  canHt->cd();
  hStackHt->Draw("HIST");
  leg->Draw("same");
  canHt->SetLogy();
  gPad->RedrawAxis();
  canHt->SaveAs("hWJets_HTbinned_Ht."+graphicsFormat);

  TCanvas* canMht = new TCanvas("canMht","Mht",canSize,canSize);
  canMht->cd();
  hStackMht->Draw("HIST");
  leg->Draw("same");
  canMht->SetLogy();
  gPad->RedrawAxis();
  canMht->SaveAs("hWJets_HTbinned_Mht."+graphicsFormat);

}
