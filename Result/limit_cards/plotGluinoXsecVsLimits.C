
#include "../../Utils/Sample.h"
#include "../../Utils/StyleMatters.h"
#include "../../Utils/HistReader.h"

// === Main Function ===================================================
void plotGluinoXsecVsLimits(const TString &graphicsFormat = "pdf"){

  StyleMatters::init();
  const int canSize = 600;

  // Define inputs
  // for number of observed events in data in search regions
  const int NMglu = 4;
  double Mglu[NMglu] = {1300.,1400.,1500.,1600.};
  double XsecGlGl[NMglu] = {0.0460525, 0.0252977, 0.0141903, 0.00810078};
  // 

  // Check these!!!!
  // fake numbers to be replaced with real numbers you get from exercise 
  double LimitsObs[NMglu]={0.4,0.7,1.0366,1.5}; 
  double LimitsExp[NMglu]={0.5,0.9,1.3477,2.0}; 
  double LimitsExpOneSigmaUp[NMglu]={0.5*1.5,0.9*1.5,1.3477*1.5,2.0*1.5}; 
  double LimitsExpOneSigmaDn[NMglu]={0.5*0.6,0.9*0.6,1.3477*0.6,2.0*0.6}; 
  // Check down to here

  //
  // Won't have to touch things below this line
  //

  // Setup TH1F for a frame
  TH1F *hFrame = new TH1F("hFrame",";Gluino mass (GeV/c^{2});#sigma(pp#rightarrow#tilde{g}#tilde{g}) [pb]",100.,1250.,1650.);
  hFrame->SetMinimum(0.005);
  hFrame->SetMaximum(0.200);
  hFrame->GetXaxis()->SetNdivisions(505);

  // Setup TGraph for xsec and limits  
  TGraph *grXsecGlGl = new TGraph(NMglu,Mglu,XsecGlGl);
  grXsecGlGl->SetLineWidth(3);
  grXsecGlGl->SetLineColor(4);
   
  double XsecLimitsObs[NMglu]; // observed limits
  double XsecLimitsExp[NMglu]; // expected limits
  double XsecLimitsExpOneSigmaUp[NMglu]; // expected limits
  double XsecLimitsExpOneSigmaDn[NMglu]; // expected limits
  double XsecLimitsExpOneSigma[NMglu*2]; // expected limits
  for (int ibin=1; ibin<=NMglu; ibin++){
    XsecLimitsObs[ibin-1] = XsecGlGl[ibin-1]*LimitsObs[ibin-1];
    XsecLimitsExp[ibin-1] = XsecGlGl[ibin-1]*LimitsExp[ibin-1];
    XsecLimitsExpOneSigmaUp[ibin-1] = XsecGlGl[ibin-1]*LimitsExpOneSigmaUp[ibin-1];
    XsecLimitsExpOneSigmaDn[ibin-1] = XsecGlGl[ibin-1]*LimitsExpOneSigmaDn[ibin-1];
    XsecLimitsExpOneSigma[ibin-1] = XsecGlGl[ibin-1]*LimitsExpOneSigmaUp[ibin-1];
    XsecLimitsExpOneSigma[NMglu*2-ibin] = XsecGlGl[ibin-1]*LimitsExpOneSigmaDn[ibin-1];
  }
  double Mglu2[NMglu*2] = {1300.,1400.,1500.,1600.,1600.,1500.,1400.,1300.};
  TGraph *grXsecGlGlLimitsObs = new TGraph(NMglu,Mglu,XsecLimitsObs);
  grXsecGlGlLimitsObs->SetLineWidth(3);
  TGraph *grXsecGlGlLimitsExp = new TGraph(NMglu,Mglu,XsecLimitsExp);
  grXsecGlGlLimitsExp->SetLineWidth(3);
  grXsecGlGlLimitsExp->SetLineColor(4);
  grXsecGlGlLimitsExp->SetLineStyle(2);
  TGraph *grXsecGlGlLimitsExpOneSigmaUp = new TGraph(NMglu,Mglu,XsecLimitsExpOneSigmaUp);
  grXsecGlGlLimitsExpOneSigmaUp->SetLineWidth(3);
  grXsecGlGlLimitsExpOneSigmaUp->SetLineColor(4);
  grXsecGlGlLimitsExpOneSigmaUp->SetLineStyle(3);
  TGraph *grXsecGlGlLimitsExpOneSigmaDn = new TGraph(NMglu,Mglu,XsecLimitsExpOneSigmaDn);
  grXsecGlGlLimitsExpOneSigmaDn->SetLineWidth(3);
  grXsecGlGlLimitsExpOneSigmaDn->SetLineColor(4);
  grXsecGlGlLimitsExpOneSigmaDn->SetLineStyle(3);
  TGraph *grXsecGlGlLimitsExpOneSigma = new TGraph(NMglu*2,Mglu2,XsecLimitsExpOneSigma);
  grXsecGlGlLimitsExpOneSigma->SetFillColor(5);

  const int kNlines=4;
  TLegend* leg = new TLegend(0.55,(0.89-0.06*(kNlines)),0.9,0.89);
  leg->SetBorderSize(0);
  leg->SetFillColor(0);
  leg->SetFillStyle(0);
  leg->SetTextFont(42);
  leg->AddEntry(grXsecGlGlLimitsObs,"95\% CL limit","L");
  leg->AddEntry(grXsecGlGlLimitsExp,"Median expected","L");
  leg->AddEntry(grXsecGlGlLimitsExpOneSigma,"68% expected","F");
  leg->AddEntry(grXsecGlGl,"#sigma(pp#rightarrow#tilde{g}#tilde{g}, #tilde{g}#rightarrow t#bar{t}#tilde{#chi}_{1}^{0})","L");

  //
  // Plotting 
  TCanvas* canGluinoXsecVsLimits = new TCanvas("canGluinoXsecVsLimits","GluinoXsecVsLimits",canSize,canSize);
  canGluinoXsecVsLimits->cd();
  hFrame->Draw();
  grXsecGlGlLimitsExpOneSigma->Draw("f");
  grXsecGlGlLimitsObs->Draw("C");
  grXsecGlGlLimitsExp->Draw("C");
  grXsecGlGl->Draw("C");
  leg->Draw("same");
  canGluinoXsecVsLimits->SetLogy();
  gPad->RedrawAxis();

  TLatex Tl; Tl.SetTextFont(43); Tl.SetTextSize(25);
  Tl.DrawText(1260., .13, "T1tttt:");
  Tl.DrawLatex(1280., .10,"pp#rightarrow#tilde{g}#tilde{g}, #tilde{g}#rightarrow t#bar{t}#tilde{#chi}_{1}^{0}");
  Tl.DrawLatex(1280., .075,"m_{#tilde{#chi}_{1}^{0}} = 100 GeV");
  Tl.SetTextSize(30);
  Tl.DrawLatex(1450., .22, "13 TeV, 2.3 fb^{-1}");

  canGluinoXsecVsLimits->SaveAs("hGluinoXsecVsLimits."+graphicsFormat);
 
}
