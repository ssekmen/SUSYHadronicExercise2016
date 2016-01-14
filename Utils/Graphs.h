#include "TGraphAsymmErrors.h"
#include "TH1.h"
#include "Math/QuantFuncMathCore.h"

const double alpha = 1 - 0.6827;

TGraphAsymmErrors* GetDataTGraph(TH1* hdata_obs) {


    
  const int nbins=6-1;
  
  Double_t x[nbins];
  Double_t xl[nbins];
  Double_t xh[nbins];

  
  Double_t data_cv[nbins];
  Double_t data_pois_up[nbins];
  Double_t data_pois_down[nbins];


  for (int bin(0); bin<nbins; bin++) {
      x[bin] = hdata_obs->GetBinCenter(bin+1);
      xl[bin]=hdata_obs->GetBinWidth(bin+1)/10000.;
      xh[bin]=hdata_obs->GetBinWidth(bin+1)/10000.;
      data_cv[bin]=hdata_obs->GetBinContent(bin+1);
      double N=data_cv[bin];
      double L =  (N==0) ? 0  : (ROOT::Math::gamma_quantile(alpha/2,N,1.));
      double U =  ROOT::Math::gamma_quantile_c(alpha/2,N+1,1) ;
      data_pois_up[bin]=(U-N);
      data_pois_down[bin]=(N-L);
  }

  TGraphAsymmErrors* gdata_obs = new TGraphAsymmErrors(nbins, x, data_cv, xl, xh, data_pois_down, data_pois_up);

  return gdata_obs;

  
}

TGraphAsymmErrors* GetDataRatioTGraph(TGraphAsymmErrors* gdata_obs, TH1* hdata_obs, TH1* hLostLep, TH1* hHadTau, TH1* hZinv, TH1* hQCD) {

  TGraphAsymmErrors* ratioderr = new TGraphAsymmErrors(5, gdata_obs->GetX(), gdata_obs->GetY(), gdata_obs->GetEXlow(), gdata_obs->GetEXhigh(), gdata_obs->GetEYlow(), gdata_obs->GetEYhigh());

  TH1D* hbg_pred = new TH1D("hbg_pred",";Search Bins;Events",6,0.5,6.5);
  hbg_pred->Add(hLostLep);
  hbg_pred->Add(hHadTau);
  hbg_pred->Add(hZinv);
  hbg_pred->Add(hQCD);
 
  for (Int_t i = 0; i < 5; i++) {
    if (hbg_pred->GetBinContent(i+1)>0) {
      ratioderr->SetPoint(i, i+1, (hdata_obs->GetBinContent(i+1)-hbg_pred->GetBinContent(i+1))/hbg_pred->GetBinContent(i+1));
      ratioderr->SetPointError(i, 0.5, 0.5, gdata_obs->GetErrorYlow(i)/hbg_pred->GetBinContent(i+1), gdata_obs->GetErrorYhigh(i)/hbg_pred->GetBinContent(i+1));
    }
  }

  gdata_obs->Print("all");
  hbg_pred->Print("all");
  
  return ratioderr;
					     
}
