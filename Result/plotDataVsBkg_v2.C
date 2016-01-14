
#include "../Utils/Sample.h"
#include "../Utils/StyleMatters.h"
#include "../Utils/HistReader.h"
#include "../Utils/Graphs.h"

// === Helper Functions ================================================
// Set the histogram style according to the sample id
void setStyle(TH1* h, unsigned int id) {
  h->SetLineWidth(2);
  //h->SetLineColor(Sample::color(id));
  h->SetLineColor(kBlack);
  h->SetMarkerColor(Sample::color(id));
  if( id < 10 ) {               // This is a data sample
    h->SetMarkerStyle(20);
  } else {
    h->SetMarkerStyle(1);
    h->SetFillColor(Sample::color(id));
  }
}

void setStyle(TGraphAsymmErrors* g, unsigned int id) {
  g->SetLineWidth(2);
  //g->SetLineColor(Sample::color(id));
  g->SetLineColor(kBlack);
  g->SetMarkerColor(Sample::color(id));
  if( id < 10 ) {               // This is a data sample
    g->SetMarkerStyle(20);
  } else {
    g->SetMarkerStyle(1);
    g->SetFillColor(Sample::color(id));
  }
}

void setLegendStyle(TLegend* leg, float text_size=0.045, bool transparent=false) {
  if (transparent) leg->SetFillStyle(0);
  leg->SetFillStyle(0);
  leg->SetLineColor(0);
  leg->SetShadowColor(0);
  leg->SetTextFont(62);
  leg->SetTextSize(text_size);
  leg->SetBorderSize(0);
  return;
}
    
// === Main Function ===================================================
void plotDataVsBkg(const TString &graphicsFormat = "pdf"){

  StyleMatters::init();
  gStyle->SetEndErrorSize(0);

  const int canSize = 500;

  // Define inputs
  // for number of observed events in data in search regions
  const int Nbins = 6;
  double Nobs[Nbins] = {53.,9.,1.,8.,1.,0.};
  // 
  // for number of estimated backgrounds
  // check these numbers
  double NLostLepton[Nbins] = {19.35160, 6.06048,0.00,1.99132,0.56454,0.0};
  double LostLeptonStatUp[Nbins] = {2.6,1.5,1.14,1.1,1.6,1.05};
  double LostLeptonStatDn[Nbins] = {2.5,1.3,0.00,0.9,1.4,0.13};
  double LostLeptonSystUp[Nbins] = {1.7,0.5,0.08,0.3,0.3,0.05};
  double LostLeptonSystDn[Nbins] = {1.7,0.5,0.01,0.3,0.3,0.00};
  // check ends

  double NHadronicTau[Nbins]     = {28.6,8.1,0.25,5.1,2.2,0.01};
  double NHadronicTauStatUp[Nbins]     = {2.7,1.6,1.13,1.5,1.1,1.13};
  double NHadronicTauStatDn[Nbins]     = {2.6,1.4,0.15,1.2,0.8,0.01};
  double NHadronicTauSystUp[Nbins]     = {1.5,0.7,0.08,0.3,0.2,0.00};
  double NHadronicTauSystDn[Nbins]     = {1.5,0.7,0.01,0.3,0.2,0.00};

  // check these numbers
  double NZinv[Nbins]       = {5.414,1.511,0.155,0.000,0.000,0.000};
  double NZinvStatUp[Nbins]       = {1.313,1.068,0.155,0.376,0.317,0.259};
  double NZinvStatDn[Nbins]       = {1.313,1.068,0.155,0.000,0.000,0.000};
  double NZinvSystUp[Nbins]       = {1.209,0.657,0.081,0.104,0.052,0.034};
  double NZinvSystDn[Nbins]       = {1.060,0.631,0.067,0.091,0.048,0.030};
  // check ends

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

  TGraphAsymmErrors* gData = GetDataTGraph(hData);
  setStyle(gData,1);


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


  TH1F *hdummy  = new TH1F("hdummy","hdummy",6,0.5,6.5);

  THStack* hBkgYieldsStack = new THStack("hBkgYieldsStack",";Search bins;Events");
  for (int ibin=1;ibin<=Nbins;ibin++){
    hLostLepton->SetBinContent(ibin,NLostLepton[ibin-1]);
    hHadronicTau->SetBinContent(ibin,NHadronicTau[ibin-1]);
    hZinv->SetBinContent(ibin,NZinv[ibin-1]);
    hQCD->SetBinContent(ibin,NQCD[ibin-1]);    
  }
  setStyle(hLostLepton,123);
  setStyle(hHadronicTau,124);
  setStyle(hZinv,122);
  setStyle(hQCD,121);
  hBkgYieldsStack->Add(hQCD);
  hBkgYieldsStack->Add(hZinv);
  hBkgYieldsStack->Add(hHadronicTau);
  hBkgYieldsStack->Add(hLostLepton);
  //  hBkgYieldsStack->Print("all");

    // // Setup legends
  TLegend * legdata;
  TLegend * leg1;
  TLegend * leg2;
  TLegend * leg3;
  TLegend * leg4;
  legdata = new TLegend(0.21-0.08, 0.75, 0.41-0.12, 0.85);
  leg1 = new TLegend(0.25, 0.756, 0.54, 0.85);
  leg2 = new TLegend(0.42, 0.756, 0.71, 0.85);
  leg3 = new TLegend(0.63, 0.756, 0.92, 0.85);
  leg4 = new TLegend(0.79, 0.756, 1.08, 0.85);

  setLegendStyle(legdata,0.04);
  setLegendStyle(leg1,0.04);
  setLegendStyle(leg2,0.04);
  setLegendStyle(leg3,0.04);
  setLegendStyle(leg4,0.04);
  legdata->AddEntry(hData, "Data", "pe");
  leg1->AddEntry(hLostLepton, "#splitline{Lost}{lepton}", "f");
  leg2->AddEntry(hHadronicTau, "#splitline{Hadronic}{#tau lepton}", "f");
  leg3->AddEntry(hZinv, "Z#rightarrow#nu#bar{#nu}", "f");
  leg4->AddEntry(hQCD, "QCD", "f");
  
  
  // Setup canvas and pads

  int W = 800;
  int H = 600;

  
  // Simple example of macro: plot with CMS name and lumi text
  //  (this script does not pretend to work in all configurations)
  // iPeriod = 1*(0/1 7 TeV) + 2*(0/1 8 TeV)  + 4*(0/1 13 TeV) 
  // For instance: 
  //               iPeriod = 3 means: 7 TeV + 8 TeV
  //               iPeriod = 7 means: 7 TeV + 8 TeV + 13 TeV 
  // Initiated by: Gautier Hamel de Monchenault (Saclay)
  // Updated by:   Dinko Ferencek (Rutgers)
  //
  int H_ref = 600; 
  int W_ref = 800; 

  // references for T, B, L, R
  float T = 0.08*H_ref;
  float B = 0.12*H_ref; 
  float L = 0.12*W_ref;
  float R = 0.04*W_ref;
  
  //
  // Plotting 
  TCanvas* canv = new TCanvas("canvName","canvName",50,50,W,H);
  canv->SetFillColor(0);
  canv->SetBorderMode(0);
  canv->SetFrameFillStyle(0);
  canv->SetFrameBorderMode(0);
  canv->SetLeftMargin( L/W );
  canv->SetRightMargin( R/W );
  canv->SetTopMargin( T/H );
  canv->SetBottomMargin( B/H );
  canv->SetTickx(0);
  canv->SetTicky(0);

  double up_height     = 0.8;  // please tune so that the upper figures size will meet your requirement
  double dw_correction = 1.30; // please tune so that the smaller canvas size will work in your environment
  double font_size_dw  = 0.1;  // please tune the font size parameter for bottom figure
  double dw_height     = (1. - up_height) * dw_correction;
  double dw_height_offset = 0.02; // KH, added to put the bottom one closer to the top panel

  
  TPad * pad1 = new TPad("pad1", "top pad" , 0.0, 0.3, 1.0, 1.0);
  TPad * pad2 = new TPad("pad2", "bottom pad", 0.0, 0.0, 1.0, 0.3);
  
  pad1->SetTickx(0);
  pad1->SetTicky(0);
  pad1->SetPad(0., 1 - up_height,    1., 1.00);
  //
  pad1->SetFrameFillColor(0);
  pad1->SetFillColor(0);
  pad1->SetTopMargin(0.12);
  pad1->SetLeftMargin(0.1);
  pad1->Draw();

  pad2->SetPad(0., 0., 1., dw_height+dw_height_offset);
  pad2->SetFillColor(0);
  pad2->SetFrameFillColor(0);
  pad2->SetBottomMargin(0.35);
  pad2->SetTopMargin(0);
  pad2->SetLeftMargin(0.1);
  pad2->Draw();
  pad1->cd();
  pad1->SetLogy();
  hBkgYieldsStack->SetMaximum(5000.);
  hBkgYieldsStack->Draw("HIST");
  //  hData->Draw("PESAME");
  gData->Draw("PSAME");
  legdata->Draw("same");
  leg1->Draw("same");
  leg2->Draw("same");
  leg3->Draw("same");
  leg4->Draw("same");
  canv->SetLogy();
  gPad->RedrawAxis();

  float ymax_top = 200;
  float ymin_top = 0.0025;
  
  TLine *tl_njet = new TLine();
  tl_njet->SetLineStyle(2);
  tl_njet->DrawLine( 3.5,ymin_top, 3.5,ymax_top);

  // Njet labels
  TLatex * ttext_njet = new TLatex();
  ttext_njet->SetTextFont(42);
  ttext_njet->SetTextSize(0.05);
  ttext_njet->SetTextAlign(22);
  ttext_njet->DrawLatex(1.2, 200. , "7 #leq N_{jets} #leq 8");
  ttext_njet->DrawLatex(4, 200. , "N_{jets} #geq 9");

  pad2->cd();
  pad2->SetGridy(0);


  TH1D * ratio = (TH1D *) hData->Clone("ratio");
  ratio->Reset();
  ratio->SetMaximum(0.6);
  ratio->SetMinimum(-0.6);
  ratio->GetXaxis()->SetLabelSize(0.12);
  ratio->GetXaxis()->SetTitleSize(0.14);
  ratio->GetXaxis()->SetTitleOffset(1.1);
  ratio->GetXaxis()->SetTitleFont(42);
  ratio->GetYaxis()->SetLabelSize(0.13);
  ratio->GetYaxis()->SetTitleSize(0.13);
  ratio->GetYaxis()->SetTitleOffset(0.32);
  ratio->GetYaxis()->SetTitleFont(42);
  ratio->GetXaxis()->SetTitle("");
  ratio->GetYaxis()->SetTitle("#frac{Obs.-Pred.}{Pred.}");
  ratio->GetYaxis()->SetNdivisions(505);
  ratio->GetYaxis()->SetTickLength(0.015);
  ratio->GetXaxis()->SetTickLength(0.08);

  TString cut_labels[6] = {"#splitline{N_{b-jet} = 2}{200 < H_{T}^{ miss} < 500}","#splitline{N_{b-jet} #geq 3}{200 < H_{T}^{ miss} < 500}","#splitline{N_{b-jet} #geq 2}{H_{T}^{ miss} > 500}",
			"#splitline{N_{b-jet} = 2}{200 < H_{T}^{ miss} < 500}","#splitline{N_{b-jet} #geq 3}{200 < H_{T}^{ miss} < 500}","#splitline{N_{b-jet} #geq 2}{H_{T}^{ miss} > 500}"};
  for (int i=1;i<=6;i++) {
    ratio->GetXaxis()->SetBinLabel(i,cut_labels[i-1].Data());
  }
  
  TGraphAsymmErrors* gRatio = GetDataRatioTGraph(gData, hData, hLostLepton, hHadronicTau, hZinv, hQCD);
  setStyle(gRatio,1);

  ratio->Draw();
  TLine* ratiounity = new TLine(0.5,1,6.5,1);
  ratiounity->SetLineStyle(2);
    
  ratiounity->Draw();
  gRatio->Draw("PSAME");
  gPad->RedrawAxis();

  
  canv->SaveAs("hDataVsBkg."+graphicsFormat);
  
}
