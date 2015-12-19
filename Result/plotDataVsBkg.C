// Compare the sum of the HT, MHT, and N(jets) distributions
// for the background to the observed distributions in
// data. For the W+jets and ttbar backgrounds, the combined
// lost-lepton and hadronic-tau prediction are used.
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
  } else if( id < 20 ) {	// This is SM background
    h->SetMarkerStyle(1);
    h->SetFillColor(Sample::color(id));
  }
}
    


// === Main Function ===================================================
void plotDataVsBkg(const TString &graphicsFormat = "png") {
  gROOT->ProcessLine(".L ../Utils/Sample.h+");
  gROOT->ProcessLine(".L ../Utils/StyleMatters.h+");
  gROOT->ProcessLine(".L ../Utils/HistReader.h+");
  StyleMatters::init();
  const int canSize = 500;

  // Loop over distributions
  for(unsigned int d = 0; d < 3; ++d) {
    TString dName("");
    if(      d == 0 ) dName = "Ht";
    else if( d == 1 ) dName = "Mht";
    else if( d == 2 ) dName = "NJets";

    // Get the data distribution
    const TString dataFileName = "General_"+Sample::toTString(1)+"-Yields.root";
    TH1* hData = HistReader::get(dataFileName,"h"+dName);
    setStyle(hData,1);

    // Get the data-based bkg distributions
    TH1* hLostLepton = HistReader::get("LostLepton_Data_Prediction.root","h"+dName+"Pred");
    hLostLepton->SetLineWidth(2);
    hLostLepton->SetMarkerStyle(1);
    hLostLepton->SetLineColor(kBlue-3);
    hLostLepton->SetMarkerColor(hLostLepton->GetLineColor());
    hLostLepton->SetFillColor(hLostLepton->GetLineColor());

    TH1* hHadTau = HistReader::get("HadTau_Data_Prediction.root","hPred"+dName);
    hHadTau->SetLineWidth(2);
    hHadTau->SetMarkerStyle(1);
    hHadTau->SetLineColor(kBlue+2);
    hHadTau->SetMarkerColor(hHadTau->GetLineColor());
    hHadTau->SetFillColor(hHadTau->GetLineColor());

    // Get the MC bkg distributions 
    const TString zinvFileName = "General_"+Sample::toTString(13)+"-Yields.root";
    TH1* hZInv = HistReader::get(zinvFileName,"h"+dName);
    setStyle(hZInv,13);

    const TString qcdFileName = "General_"+Sample::toTString(14)+"-Yields.root";
    TH1* hQCD = HistReader::get(qcdFileName,"h"+dName);
    setStyle(hQCD,14);

    // Create background stack
    TString xTitle("");
    if(      d == 0 ) xTitle = "H_{T} [GeV]";
    else if( d == 1 ) xTitle = "#slash{H}_{T} [GeV]";
    else if( d == 2 ) xTitle = "N(jets)";
    THStack* hBkgs = new THStack("hBkgs",";"+xTitle+";Events");
    hBkgs->Add(hQCD);
    hBkgs->Add(hLostLepton);
    hBkgs->Add(hHadTau);
    hBkgs->Add(hZInv);
    if( d < 2 ) hZInv->GetXaxis()->SetNdivisions(505);

    // Get signal distributions
    const TString lm6FileName = "General_"+Sample::toTString(21)+"-Yields.root";
    TH1* hLM6 = HistReader::get(lm6FileName,"h"+dName);
    setStyle(hLM6,21);

    const TString lm9FileName = "General_"+Sample::toTString(22)+"-Yields.root";
    TH1* hLM9 = HistReader::get(lm9FileName,"h"+dName);
    setStyle(hLM9,22);
    

    // Create legend
    TLegend* leg = new TLegend(0.6,(0.89-0.05*(7)),0.9,0.89);
    leg->SetBorderSize(0);
    leg->SetFillColor(0);
    leg->SetFillStyle(0);
    leg->SetTextFont(42);
    leg->AddEntry(hData,Sample::label(1),"P");
    leg->AddEntry(hZInv,Sample::label(13),"F");
    leg->AddEntry(hHadTau,"Hadronic-#tau","F");
    leg->AddEntry(hLostLepton,"Lost e/#mu","F");
    leg->AddEntry(hQCD,Sample::label(14),"F");
    leg->AddEntry(hLM6,Sample::label(21),"L");
    leg->AddEntry(hLM9,Sample::label(22),"L");
    
    // Draw stuff
    TCanvas* can = new TCanvas("can"+dName,dName,canSize,canSize);
    can->cd();
    hBkgs->Draw("HIST");
    hLM6->Draw("HISTsame");
    hLM9->Draw("HISTsame");
    hData->Draw("PE1same");
    leg->Draw("same");
    can->SetLogy();
    gPad->RedrawAxis();
    can->SaveAs("hDataVsBkg_"+dName+"."+graphicsFormat);
  }
}
