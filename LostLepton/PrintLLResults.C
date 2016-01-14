/* 
   Script for drawing a branch from reduec trees with a set of cuts.
*/

#include <iostream>
#include <vector>
#include <math.h> 
#include "TROOT.h"
#include "TFile.h"
#include "TSystem.h"
#include "TChain.h"
#include "TTree.h"
#include "TCut.h"
#include "THStack.h"
#include "TLine.h"
#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"
#include "TFileCollection.h"
#include "TLorentzVector.h"
#include "TPaveText.h"
#include "TGraphAsymmErrors.h"

using namespace std;


void PrintLLResults() {

  TH1::SetDefaultSumw2(1);
  //gROOT->SetBatch(1);

  TFile* infile = new TFile("LLPrediction.root", "read");
  TH1D* hin = (TH1D*)infile->Get("Prediction_data/totalPred_LL");
  TH1D* hnonclosureup = (TH1D*)infile->Get("Prediction_data/totalPredNonClosureUp_LL");
  TH1D* hnonclosuredown = (TH1D*)infile->Get("Prediction_data/totalPredNonClosureDown_LL");
  
  TProfile* hAvgWeight = (TProfile*)infile->Get("Prediction_MC/avgWeight_LL_MC");
  
  TFile* outfile = new TFile("LLGraphs.root", "recreate");
  outfile->cd();

  TH1D* hPredAllBins = new TH1D("hPredAllBins", ";Search Bin;Events / Bin", 6, 0.5, 6.5);
  Double_t pred_cv[6];
  Double_t x[6];
  Double_t xl[6];
  Double_t xh[6];
  Double_t stat_up[6];
  Double_t stat_down[6];
  Double_t syst_up[6];
  Double_t syst_down[6];
  Double_t full_err_up[6];
  Double_t full_err_down[6];
  printf("Bin #:    CV +\t statUp +  systUp -  statDown - systDown\n");

  for (unsigned int bin(0); bin<6; bin++) {
    hPredAllBins->SetBinContent(bin+1, hin->GetBinContent(bin+1));
    pred_cv[bin] = hin->GetBinContent(bin+1);
    x[bin] = bin+1;
    xl[bin]=0.5;
    xh[bin]=0.5;
    stat_up[bin] = pow(hAvgWeight->GetBinContent(bin+1)*1.84102, 2.); // the PoisCl(0) term
    stat_up[bin] += pow(hin->GetBinError(bin+1), 2.); // Sumw2
    stat_down[bin] = pow(hin->GetBinError(bin+1), 2.); // Sumw2
    syst_up[bin] = 0.;
    syst_down[bin] = 0.;
    syst_up[bin] += pow((hnonclosureup->GetBinContent(bin+1)-1.)*hin->GetBinContent(bin+1), 2.);
    syst_down[bin] += pow((1.-hnonclosuredown->GetBinContent(bin+1))*hin->GetBinContent(bin+1), 2.);

    stat_up[bin]=sqrt(stat_up[bin]);
    stat_down[bin]=sqrt(stat_down[bin]);
    syst_up[bin]=sqrt(syst_up[bin]);
    syst_down[bin]=sqrt(syst_down[bin]);
    if (stat_down[bin]==pred_cv[bin]) syst_down[bin]=0;
    if (stat_down[bin]+syst_down[bin]>pred_cv[bin]) syst_down[bin]=pred_cv[bin]-stat_down[bin];

    full_err_up[bin]=sqrt(stat_up[bin]*stat_up[bin]+syst_up[bin]*syst_up[bin]);
    full_err_down[bin]=sqrt(stat_down[bin]*stat_down[bin]+syst_down[bin]*syst_down[bin]);
    printf("Bin %d: %3.5f + %3.5f + %3.5f - %3.5f - %3.5f\n", bin+1, pred_cv[bin], stat_up[bin], syst_up[bin], stat_down[bin], syst_down[bin]);
  }

  TGraphAsymmErrors* gFull = new TGraphAsymmErrors(6, x, pred_cv, xl, xh, full_err_down, full_err_up);
  TGraphAsymmErrors* gStat = new TGraphAsymmErrors(6, x, pred_cv, xl, xh, stat_down, stat_up);
  TGraphAsymmErrors* gSyst = new TGraphAsymmErrors(6, x, pred_cv, xl, xh, syst_down, syst_up);
  gFull->Write();
  gStat->Write();
  gSyst->Write();
  hPredAllBins->Write();
  outfile->Close();

  
  return;
  
}

