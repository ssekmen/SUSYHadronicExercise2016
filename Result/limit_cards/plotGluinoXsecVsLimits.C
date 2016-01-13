
#include "../../Utils/Sample.h"
#include "../../Utils/StyleMatters.h"
#include "../../Utils/HistReader.h"

// === Main Function ===================================================
void plotGluinoXsecVsLimits(const TString &graphicsFormat = "png"){

  StyleMatters::init();
  const int canSize = 500;

  // Define inputs
  // for number of observed events in data in search regions
  const int NMglu = 4;
  double Mglu[NMglu] = {1300.,1400.,1500.,1600.};
  double XsecGlGl[NMglu] = {0.0460525, 0.0252977, 0.0141903, 0.00810078};
  // 

  // fake numbers to be replaced with real numbers you get from exercise
  double LimitsObs[NMglu]={0.4,0.7,1.0366,1.5}; 
  double LimitsExp[NMglu]={0.5,0.9,1.3477,2.0}; 
  double LimitsExpOneSigmaUp[NMglu]={0.5*1.5,0.9*1.5,1.3477*1.5,2.0*1.5}; 
  double LimitsExpOneSigmaDn[NMglu]={0.5*0.6,0.9*0.6,1.3477*0.6,2.0*0.6}; 
  
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
  for (int ibin=1; ibin<=NMglu; ibin++){
    XsecLimitsObs[ibin-1] = XsecGlGl[ibin-1]*LimitsObs[ibin-1];
    XsecLimitsExp[ibin-1] = XsecGlGl[ibin-1]*LimitsExp[ibin-1];
    XsecLimitsExpOneSigmaUp[ibin-1] = XsecGlGl[ibin-1]*LimitsExpOneSigmaUp[ibin-1];
    XsecLimitsExpOneSigmaDn[ibin-1] = XsecGlGl[ibin-1]*LimitsExpOneSigmaDn[ibin-1];
  }
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

  const int kNlines=4;
  TLegend* leg = new TLegend(0.5,(0.89-0.06*(kNlines)),0.9,0.89);
  leg->SetBorderSize(0);
  leg->SetFillColor(0);
  leg->SetFillStyle(0);
  leg->SetTextFont(42);
  leg->AddEntry(grXsecGlGlLimitsObs,"95\% CL limit","L");
  leg->AddEntry(grXsecGlGlLimitsExp,"Median expected","L");
  leg->AddEntry(grXsecGlGlLimitsExpOneSigmaUp,"68% expected","L");
  leg->AddEntry(grXsecGlGl,"#sigma(pp#rightarrow#tilde{g}#tilde{g})","L");
   
  //
  // Plotting 
  TCanvas* canGluinoXsecVsLimits = new TCanvas("canGluinoXsecVsLimits","GluinoXsecVsLimits",canSize,canSize);
  canGluinoXsecVsLimits->cd();
  hFrame->Draw();
  grXsecGlGl->Draw("C");
  grXsecGlGlLimitsObs->Draw("C");
  grXsecGlGlLimitsExp->Draw("C");
  grXsecGlGlLimitsExpOneSigmaUp->Draw("C");
  grXsecGlGlLimitsExpOneSigmaDn->Draw("C");
  leg->Draw("same");
  canGluinoXsecVsLimits->SetLogy();
  gPad->RedrawAxis();
  canGluinoXsecVsLimits->SaveAs("hGluinoXsecVsLimits."+graphicsFormat);
  
}
