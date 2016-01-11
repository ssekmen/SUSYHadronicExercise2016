#include "TVector2.h"
#include "TH2.h"
#include <cmath>
#include <iostream>

static double deltaR(double eta1, double phi1, double eta2, double phi2)
{
  double deta = eta1-eta2;
  double dphi = TVector2::Phi_mpi_pi(phi1-phi2);
  return sqrt(deta * deta + dphi *dphi); 
}

static double MTWCalculator(double metPt,double  metPhi,double  lepPt,double  lepPhi, int scaleMet = 0)
{
  if(scaleMet == 1) metPt = metPt * 1.3;
  if(scaleMet == -1) metPt = metPt * 0.7;
  double deltaPhi =TVector2::Phi_mpi_pi(lepPhi-metPhi);
  return sqrt(2*lepPt*metPt*(1-cos(deltaPhi)) );
}

static double PTWCalculator(double metPt,double  metPhi,double  lepPt,double  lepPhi, int scaleMet = 0)
{
  if(scaleMet == 1) metPt = metPt * 1.3;
  if(scaleMet == -1) metPt = metPt * 0.7;
  TVector2 Wvec(metPt*cos(metPhi)+lepPt*cos(lepPhi), metPt*sin(metPhi)+lepPt*sin(lepPhi));
  return Wvec.Mod();
}

static double GetTriggerEffWeight(Double_t MHT){
  if (MHT<50) return 0.0080;
  else if (MHT<100) return 0.0988;
  else if (MHT<150) return 0.5424;
  else if (MHT<200) return 0.9011;
  else if (MHT<250) return 0.9846;
  else if (MHT<300) return 0.9961;
  else if (MHT<400) return 0.9865;
  else if (MHT<500) return 1.0000;
  else return 1.0000;
}

static double GetSignalTriggerEffWeight(Double_t MHT) {
    if (MHT<200) return 0;
    else if (MHT<500) return 0.9819;
    else return 0.9833;
}

static double GetSF(TH2 *hist, Double_t xVal, Double_t yVal, bool addSys) {
  // addSys: for muons, 1% systematic has to be added to total uncertainty

  // Dont use overflow bins!
  if(xVal < hist->GetXaxis()->GetXmin() )
  {
    //std::cout<<"SF: Warning xVal: "<<xVal<<" is smaller than minimum of histo: "<<hist->GetName()<<std::endl;
    xVal= hist->GetXaxis()->GetXmin()+0.01;
  }
  else if(xVal > hist->GetXaxis()->GetXmax() )
  {
    //std::cout<<"SF: Warning xVal: "<<xVal<<" is bigger than maximum of histo: "<<hist->GetName()<<" which is: "<<hist->GetXaxis()->GetXmax()<<std::endl;
    xVal= hist->GetXaxis()->GetXmax()-0.01;
  }
  
  if(yVal < hist->GetYaxis()->GetXmin() )
  {
    //std::cout<<"SF: Warning yVal: "<<yVal<<" is smaller than minimum of histo: "<<hist->GetName()<<std::endl;
    yVal= hist->GetYaxis()->GetXmin()+0.01;
  }
  else if(yVal > hist->GetYaxis()->GetXmax() )
  {
    //std::cout<<"SF: Warning yVal: "<<yVal<<" is bigger than maximum of histo: "<<hist->GetName()<<std::endl;
    yVal= hist->GetYaxis()->GetXmax()-0.01;
  }

  int nxBin = hist->GetXaxis()->FindBin(xVal);
  int nyBin = hist->GetYaxis()->FindBin(yVal);

  if(nxBin > hist->GetNbinsX() || nyBin > hist->GetNbinsY()) std::cout<<"SF: Problem in getting Efficiencies!"<<std::endl;
  if(nxBin > hist->GetNbinsX()) nxBin = hist->GetNbinsX();
  if(nyBin > hist->GetNbinsY()) nyBin = hist->GetNbinsY();

  //double SF = std::abs(1-hist->GetBinContent(nxBin, nyBin));
  double SF = 0.;

  if(addSys) SF = std::max(std::abs(1-hist->GetBinContent(nxBin, nyBin)), std::sqrt(hist->GetBinError(nxBin, nyBin)*hist->GetBinError(nxBin, nyBin) + 0.01*hist->GetBinContent(nxBin, nyBin)*0.01*hist->GetBinContent(nxBin, nyBin)));
  else SF = std::max(std::abs(1-hist->GetBinContent(nxBin, nyBin)), hist->GetBinError(nxBin, nyBin));

  //std::cout << std::abs(1-hist->GetBinContent(nxBin, nyBin)) << " " << std::sqrt(hist->GetBinError(nxBin, nyBin)*hist->GetBinError(nxBin, nyBin) + 0.01*hist->GetBinContent(nxBin, nyBin)*0.01*hist->GetBinContent(nxBin, nyBin)) << " " << hist->GetBinError(nxBin, nyBin)<<std::endl;

  return SF;
}

static double getMuonIDSF(Double_t pt, Double_t eta){
  double sf = 1.;

  if(std::abs(eta)<1.2){
    if(pt < 20) sf = 0.983;
    else if(pt < 30) sf = 0.985;
    else if(pt < 40) sf = 0.992;
    else if(pt < 50) sf = 0.994;
    else if(pt < 60) sf = 0.988;
    else if(pt < 80) sf = 0.986;
    else if(pt < 120) sf = 0.979;
    else sf = 1.016;
  }
  else{
    if(pt < 20) sf = 1.004;
    else if(pt < 30) sf = 0.994;
    else if(pt < 40) sf = 0.99;
    else if(pt < 50) sf = 0.992;
    else if(pt < 60) sf = 0.986;
    else if(pt < 80) sf = 0.981;
    else if(pt < 120) sf = 0.955;
    else sf = 1.005;
  }

  return std::abs(1. - sf);
}

static double getMuonIsoSF(Double_t pt, Double_t eta, Double_t act){
  double sf = 1.;

  if(std::abs(eta)<1.2){
    if(pt < 20) sf = 1.002;
    else if(pt < 30) sf = 1.001;
    else if(pt < 40) sf = 1.000;
    else if(pt < 50) sf = 1.000;
    else if(pt < 60) sf = 1.001;
    else if(pt < 80) sf = 1.001;
    else if(pt < 120) sf = 1.000;
    else sf = 1.000;
  }
  else{
    if(pt < 20) sf = 0.995;
    else if(pt < 30) sf = 1.001;
    else if(pt < 40) sf = 1.000;
    else if(pt < 50) sf = 1.000;
    else if(pt < 60) sf = 1.000;
    else if(pt < 80) sf = 1.000;
    else if(pt < 120) sf = 1.000;
    else sf = 1.000;
  }

  return std::abs(1. - sf);
}

static double getElecIDSF(Double_t pt, Double_t eta){
  double sf = 1.;

  if(std::abs(eta)<1.442){
    if(pt < 20) sf = 0.941;
    else if(pt < 30) sf = 0.981;
    else if(pt < 40) sf = 0.995;
    else if(pt < 50) sf = 0.996;
    else sf = 1.001;
  }else if(std::abs(eta)<1.5){
    if(pt < 20) sf = 1.135;
    else if(pt < 30) sf = 1.035;
    else if(pt < 40) sf = 0.974;
    else if(pt < 50) sf = 1.006;
    else sf = 1.000;
  }else{
    if(pt < 20) sf = 0.970;
    else if(pt < 30) sf = 0.965;
    else if(pt < 40) sf = 1.002;
    else if(pt < 50) sf = 1.016;
    else sf = 1.006;
  }

  return std::abs(1. - sf);
}

static double getElecIsoSF(Double_t pt, Double_t eta, Double_t act){
  double sf = 1.;

  if(act<0.02){
    if(pt < 20) sf = 0.988;
    else if(pt < 30) sf = 0.996;
    else if(pt < 40) sf = 0.996;
    else if(pt < 50) sf = 0.997;
    else sf = 1.001;
  }else if(act<0.05){
    if(pt < 20) sf = 0.948;
    else if(pt < 30) sf = 0.991;
    else if(pt < 40) sf = 0.997;
    else if(pt < 50) sf = 0.996;
    else sf = 0.995;
  }else if(act<0.15){
    if(pt < 20) sf = 1.007;
    else if(pt < 30) sf = 0.992;
    else if(pt < 40) sf = 0.997;
    else if(pt < 50) sf = 0.997;
    else sf = 0.996;
  }else if(act<1.){
    if(pt < 20) sf = 0.992;
    else if(pt < 30) sf = 0.972;
    else if(pt < 40) sf = 0.989;
    else if(pt < 50) sf = 1.004;
    else sf = 0.977;
  }else{
    if(pt < 20) sf = 0.826;
    else if(pt < 30) sf = 0.952;
    else if(pt < 40) sf = 1.090;
    else if(pt < 50) sf = 0.891;
    else sf = 0.930;
  }

  return std::abs(1. - sf);
}
