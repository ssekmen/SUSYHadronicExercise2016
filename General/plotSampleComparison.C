// Compare the normalised HT, MHT, and N(jets) distributions
// for the background and signal samples.
//
// Author: Matthias Schroeder
//         matthias.schroeder@AT@desy.de
//         November 2013


// === Helper Functions ================================================

// Normalise the histogram to unit area
void normHist(TH1* h) {
  if( h->Integral("width") > 0. ) h->Scale(1./h->Integral("width"));
  h->GetYaxis()->SetTitle("Probability");
}

// Set the histogram style according to the sample id
void setStyle(TH1* h, unsigned int id) {
  h->SetLineWidth(2);
  h->SetLineColor(Sample::color(id));
  h->SetMarkerColor(Sample::color(id));
  if( id < 10 ) {		// This is a data sample
    h->SetMarkerStyle(20);
  } else {
    h->SetMarkerStyle(1);
  }
}
    


// === Main Function ===================================================
void plotSampleComparison(const TString &graphicsFormat = "png") {
  gROOT->ProcessLine(".L ../Utils/Sample.h+");
  gROOT->ProcessLine(".L ../Utils/StyleMatters.h+");
  gROOT->ProcessLine(".L ../Utils/HistReader.h+");
  StyleMatters::init();
  const int canSize = 500;


  const int kNSamples = 6;
  unsigned int ids[kNSamples] = { 11, 12, 13, 14, 21, 22 };


  TH1* hNJets[kNSamples];
  TH1* hHt[kNSamples];
  TH1* hMht[kNSamples];
  const int kNJetHists = 3;
  TH1* hJetPt[kNJetHists][kNSamples];
  TH1* hJetEta[kNJetHists][kNSamples];
  TH1* hDeltaPhi[kNJetHists][kNSamples];

  for(int s = 0; s < kNSamples; ++s) { // Loop over samples
    // Get histograms from file
    const TString fileName = "General_"+Sample::toTString(ids[s])+".root";

    hNJets[s] = HistReader::get(fileName,"hNJets");
    hHt[s] = HistReader::get(fileName,"hHt");
    hMht[s] = HistReader::get(fileName,"hMht");
    // Normalise histograms because we want to compare the shapes only
    normHist(hNJets[s]);
    normHist(hHt[s]);
    normHist(hMht[s]);
    // Set style and color
    setStyle(hNJets[s],ids[s]);
    setStyle(hHt[s],ids[s]);
    setStyle(hMht[s],ids[s]);

    for(unsigned int i = 0; i < kNJetHists; ++i) {
      TString name = "hJetPt_";
      name += i;
      hJetPt[i][s] = HistReader::get(fileName,name);
      name = "hJetEta_";
      name += i;
      hJetEta[i][s] = HistReader::get(fileName,name);
      name = "hDeltaPhi_";
      name += i;
      hDeltaPhi[i][s] = HistReader::get(fileName,name);
      // Normalise histograms because we want to compare the shapes only
      normHist(hJetPt[i][s]);
      normHist(hJetEta[i][s]);
      normHist(hDeltaPhi[i][s]);
      // Set style and color
      setStyle(hJetPt[i][s],ids[s]);
      setStyle(hJetEta[i][s],ids[s]);
      setStyle(hDeltaPhi[i][s],ids[s]);
    }
  } // End of loop over samples


  
  // Draw
  TString drawOptions[kNSamples];
  for(int s = 0; s < kNSamples; ++s) {
    drawOptions[s] = ids[s] < 10 ? "PE1" : "HISTE";
  }

  // Create legend
  TLegend* leg = new TLegend(0.6,(0.89-0.05*kNSamples),0.9,0.89);
  leg->SetBorderSize(0);
  leg->SetFillColor(0);
  leg->SetFillStyle(0);
  leg->SetTextFont(42);
  for(int s = 0; s < kNSamples; ++s) {
    leg->AddEntry(hHt[s],Sample::label(ids[s]),(ids[s]<10?"P":"L"));
  }

  TCanvas* canNJets = new TCanvas("canNJets","NJets",canSize,canSize);
  canNJets->cd();
  hNJets[0]->GetYaxis()->SetRangeUser(0.,0.9);
  hNJets[0]->Draw(drawOptions[0]);
  for(int s = 1; s < kNSamples; ++s) {
    hNJets[s]->Draw(drawOptions[s]+"same");
  }
  leg->Draw("same");
  canNJets->SaveAs("hNJets."+graphicsFormat);

  TCanvas* canHt = new TCanvas("canHt","Ht",canSize,canSize);
  canHt->cd();
  hHt[0]->GetYaxis()->SetRangeUser(3E-9,9E-2);
  hHt[0]->Draw(drawOptions[0]);
  for(int s = 1; s < kNSamples; ++s) {
    hHt[s]->Draw(drawOptions[s]+"same");
  }
  leg->Draw("same");
  canHt->SetLogy();
  canHt->SaveAs("hHt."+graphicsFormat);

  TCanvas* canMht = new TCanvas("canMht","Mht",canSize,canSize);
  canMht->cd();
  hMht[0]->GetYaxis()->SetRangeUser(3E-9,9E-2);
  hMht[0]->Draw(drawOptions[0]);
  for(int s = 1; s < kNSamples; ++s) {
    hMht[s]->Draw(drawOptions[s]+"same");
  }
  leg->Draw("same");
  canMht->SetLogy();
  canMht->SaveAs("hMht."+graphicsFormat);

  for(unsigned int i = 0; i < kNJetHists; ++i) {
    TString name = "JetEta";
    name += i+1;
    TCanvas* can = new TCanvas(name,name,canSize,canSize);
    can->cd();
    hJetEta[i][0]->GetYaxis()->SetRangeUser(0,0.5);
    hJetEta[i][0]->Draw(drawOptions[0]);
    for(int s = 1; s < kNSamples; ++s) {
      hJetEta[i][s]->Draw(drawOptions[s]+"same");
    }
    leg->Draw("same");
    can->SaveAs("h"+name+"."+graphicsFormat);
  }

  for(unsigned int i = 0; i < kNJetHists; ++i) {
    TString name = "DeltaPhi";
    name += i+1;
    TCanvas* can = new TCanvas(name,name,canSize,canSize);
    can->cd();
    hDeltaPhi[i][0]->GetYaxis()->SetRangeUser(0,0.6);
    hDeltaPhi[i][0]->Draw(drawOptions[0]);
    for(int s = 1; s < kNSamples; ++s) {
      hDeltaPhi[i][s]->Draw(drawOptions[s]+"same");
    }
    leg->Draw("same");
    can->SaveAs("h"+name+"."+graphicsFormat);
  }

}
