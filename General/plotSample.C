// Plot the HT, MHT, and N(jets) distributions for the stated sample.
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
  }
}


// === Main Function ===================================================
void plotSample(unsigned int id,
		bool afterSelection = false,
		const TString &graphicsFormat = "png") {
  gROOT->ProcessLine(".L ../Utils/Sample.h+");
  gROOT->ProcessLine(".L ../Utils/StyleMatters.h+");
  gROOT->ProcessLine(".L ../Utils/HistReader.h+");
  StyleMatters::init();
  const int canSize = 500;

  std::cout << "Plotting distributions for '" << Sample::toTString(id) << "' sample" << std::endl;
  const TString fileName = afterSelection ? ("General_"+Sample::toTString(id)+"-Yields.root") : ("General_"+Sample::toTString(id)+".root");

 
  // Get histograms from file
  TH1* hNJets = HistReader::get(fileName,"hNJets");
  TH1* hHt = HistReader::get(fileName,"hHt");
  TH1* hMht = HistReader::get(fileName,"hMht");
  // Set style and color
  setStyle(hNJets,id);
  setStyle(hHt,id);
  setStyle(hMht,id);

  const int kNJetHists = 3;
  TH1* hJetPt[kNJetHists];
  TH1* hJetEta[kNJetHists];
  TH1* hDeltaPhi[kNJetHists];
  for(unsigned int i = 0; i < kNJetHists; ++i) {
    TString name = "hJetPt_";
    name += i;
    hJetPt[i] = HistReader::get(fileName,name);
    name = "hJetEta_";
    name += i;
    hJetEta[i] = HistReader::get(fileName,name);
    name = "hDeltaPhi_";
    name += i;
    hDeltaPhi[i] = HistReader::get(fileName,name);
    // Set style and color
    setStyle(hJetPt[i],id);
    setStyle(hJetEta[i],id);
    setStyle(hDeltaPhi[i],id);
  }


  
  // Draw
  const TString drawOption = id < 10 ? "PE1" : "HISTE";
  const TString outName = "h"+Sample::toTString(id)+"_";

  // Create legend
  TLegend* leg = new TLegend(0.3,0.8,0.9,0.89);
  leg->SetBorderSize(0);
  leg->SetFillColor(0);
  leg->SetFillStyle(0);
  leg->SetTextFont(42);
  leg->SetTextSize(0.05);
  char entry[100];
  sprintf(entry,"N(total) = %.1f",hHt->Integral());
  leg->AddEntry(hHt,entry,"");

  TCanvas* canNJets = new TCanvas("canNJets","NJets",canSize,canSize);
  canNJets->cd();
  hNJets->Draw(drawOption);
  leg->Draw("same");
  canNJets->SaveAs(outName+"NJets."+graphicsFormat);

  TCanvas* canHt = new TCanvas("canHt","Ht",canSize,canSize);
  canHt->cd();
  hHt->Draw(drawOption);
  leg->Draw("same");
  canHt->SetLogy();
  canHt->SaveAs(outName+"Ht."+graphicsFormat);

  TCanvas* canMht = new TCanvas("canMht","Mht",canSize,canSize);
  canMht->cd();
  hMht->Draw(drawOption);
  leg->Draw("same");
  canMht->SetLogy();
  canMht->SaveAs(outName+"Mht."+graphicsFormat);

  for(unsigned int i = 0; i < kNJetHists; ++i) {
    TString name = "JetPt";
    name += i+1;
    TCanvas* can = new TCanvas(name,name,canSize,canSize);
    can->cd();
    hJetPt[i]->Draw(drawOption);
    can->SetLogy();
    can->SaveAs(outName+name+"."+graphicsFormat);
  }

  for(unsigned int i = 0; i < kNJetHists; ++i) {
    TString name = "JetEta";
    name += i+1;
    TCanvas* can = new TCanvas(name,name,canSize,canSize);
    can->cd();
    hJetEta[i]->Draw(drawOption);
    can->SaveAs(outName+name+"."+graphicsFormat);
  }

  for(unsigned int i = 0; i < kNJetHists; ++i) {
    TString name = "DeltaPhi";
    name += i+1;
    TCanvas* can = new TCanvas(name,name,canSize,canSize);
    can->cd();
    hDeltaPhi[i]->Draw(drawOption);
    can->SaveAs(outName+name+"."+graphicsFormat);
  }
}
