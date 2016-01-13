
#include "../Utils/Sample.h"
#include "../Utils/StyleMatters.h"
#include "../Utils/HistReader.h"

// === Helper Functions ================================================
// Set the histogram style according to the sample id
void setStyle(TH1* h, unsigned int id) {
  h->SetLineWidth(2);
  h->SetLineColor(Sample::color(id));
  h->SetMarkerColor(Sample::color(id));
  if( id < 10 ) {               // This is a data sample
    h->SetMarkerStyle(20);
  } else {
    h->SetMarkerStyle(1);
    h->SetFillColor(Sample::color(id));
  }
}
    
// === Main Function ===================================================
void plotDataVsBkg(const TString &graphicsFormat = "png"){

  StyleMatters::init();
  const int canSize = 500;

  // Define inputs
  // for number of observed events in data in search regions
  const int Nbins = 6;
  double Nobs[Nbins] = {53.,9.,1.,8.,1.,0.};
  // 
  // for number of estimated backgrounds
  double NLostLepton[Nbins] = {21.2,5.0,0.00,3.1,2.7,0.13};
  double LostLeptonStatUp[Nbins] = {2.6,1.5,1.14,1.1,1.6,1.05};
  double LostLeptonStatDn[Nbins] = {2.5,1.3,0.00,0.9,1.4,0.13};
  double LostLeptonSystUp[Nbins] = {1.7,0.5,0.08,0.3,0.3,0.05};
  double LostLeptonSystDn[Nbins] = {1.7,0.5,0.01,0.3,0.3,0.00};

  double NHadronicTau[Nbins]     = {28.6,8.1,0.25,5.1,2.2,0.01};
  double NHadronicTauStatUp[Nbins]     = {2.7,1.6,1.13,1.5,1.1,1.13};
  double NHadronicTauStatDn[Nbins]     = {2.6,1.4,0.15,1.2,0.8,0.01};
  double NHadronicTauSystUp[Nbins]     = {1.5,0.7,0.08,0.3,0.2,0.00};
  double NHadronicTauSystDn[Nbins]     = {1.5,0.7,0.01,0.3,0.2,0.00};

  double NZinv[Nbins]       = {5.414,1.511,0.155,0.000,0.000,0.000};
  double NZinvStatUp[Nbins]       = {1.313,1.068,0.155,0.376,0.317,0.259};
  double NZinvStatDn[Nbins]       = {1.313,1.068,0.155,0.000,0.000,0.000};
  double NZinvSystUp[Nbins]       = {1.209,0.657,0.081,0.104,0.052,0.034};
  double NZinvSystDn[Nbins]       = {1.060,0.631,0.067,0.091,0.048,0.030};

  double NQCD[Nbins]        = {5.6,1.7,0.07,1.5,0.3,0.00};
  double NQCDStatUp[Nbins]        = {0.8,0.7,0.23,0.5,0.3,0.20};
  double NQCDStatDn[Nbins]        = {0.6,0.4,0.04,0.3,0.1,0.00};
  double NQCDSystUp[Nbins]        = {2.1,1.0,0.09,0.9,0.2,0.05};
  double NQCDSystDn[Nbins]        = {2.0,0.8,0.01,0.9,0.1,0.00};
  // 

  //
  // Won't have to touch things below this line
  //

  //
  // Setting up search region data histogram
  TH1F *hData = new TH1F("hData",";Search Bins;Events",6,0.5,6.5);
  hData->Sumw2();
  for (int ibin=1; ibin<=hData->GetNbinsX(); ibin++){
    hData->SetBinContent(ibin,Nobs[ibin-1]);
    hData->SetBinError(ibin,sqrt(Nobs[ibin-1]));
    TString label = "Bin ";
    label += ibin;
    hData->GetXaxis()->SetBinLabel(ibin,label);
  }
  setStyle(hData,1);

  //
  // Set up background histograms
  TH1F *hLostLepton  = new TH1F("hLostLepton","hLostLepton",6,0.5,6.5);
  TH1F *hHadronicTau = new TH1F("hHadronicTau","hHadronicTau",6,0.5,6.5);
  TH1F *hZinv        = new TH1F("hZinv","hZinv",6,0.5,6.5);
  TH1F *hQCD         = new TH1F("hQCD","hQCD",6,0.5,6.5);
  hLostLepton->Sumw2();
  hHadronicTau->Sumw2();
  hZinv->Sumw2();
  hQCD->Sumw2();

  THStack* hBkgYieldsStack = new THStack("hBkgYieldsStack",";Search bins;Events");
  for (int ibin=1;ibin<=Nbins;ibin++){
    hLostLepton->SetBinContent(ibin,NLostLepton[ibin-1]);
    hHadronicTau->SetBinContent(ibin,NHadronicTau[ibin-1]);
    hZinv->SetBinContent(ibin,NZinv[ibin-1]);
    hQCD->SetBinContent(ibin,NQCD[ibin-1]);    
  }
  setStyle(hLostLepton,11);
  setStyle(hHadronicTau,12);
  setStyle(hZinv,13);
  setStyle(hQCD,14);
  hBkgYieldsStack->Add(hQCD);
  hBkgYieldsStack->Add(hZinv);
  hBkgYieldsStack->Add(hHadronicTau);
  hBkgYieldsStack->Add(hLostLepton);
  hBkgYieldsStack->Print("all");

  const int kNBkgCategories = 4;
  TLegend* leg = new TLegend(0.6,(0.89-0.05*(kNBkgCategories+1)),0.9,0.89);
  leg->SetBorderSize(0);
  leg->SetFillColor(0);
  leg->SetFillStyle(0);
  leg->SetTextFont(42);
  leg->AddEntry(hData,Sample::label(1),"P");
  leg->AddEntry(hLostLepton,"Lost-lepton","F");
  leg->AddEntry(hHadronicTau,"Hadronic-tau","F");
  leg->AddEntry(hZinv,"Z(#nu#bar{#nu})+jets","F");
  leg->AddEntry(hQCD,"QCD multijets","F");
   
  //
  // Plotting 
  TCanvas* canDataVsBkg = new TCanvas("canDataVsBkg","DataVsBkg",canSize,canSize);
  canDataVsBkg->cd();
  hBkgYieldsStack->SetMaximum(1000.);
  hBkgYieldsStack->Draw("HIST");
  hData->Draw("PE1SAME");
  leg->Draw("same");
  canDataVsBkg->SetLogy();
  gPad->RedrawAxis();
  canDataVsBkg->SaveAs("hDataVsBkg."+graphicsFormat);
  
}
