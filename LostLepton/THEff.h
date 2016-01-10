#include "SearchBins.h"

#include <TChain.h>
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TTree.h>
#include <TH2D.h>
#include <TH1D.h>
#include "TVector2.h"
#include <cmath>
#include "TCanvas.h"
#include "TString.h"
#include "TEfficiency.h"
#include "TGraphAsymmErrors.h"
#include <iostream>
#include <iomanip> 

struct effVec{
	double eff;
	double errUp;
	double errDown;
};

class TH1Eff
{
public:
	TH1Eff(){}
	TH1Eff(const char* name, const char* title, Int_t nbinsx, const double* xbins)
	{
		name_=TString(name);
		title_=TString(title);
		PassTH1D_ = new TH1D(name_+"Pass", title_, nbinsx, xbins);
		PassTH1D_->Sumw2();
		TotalTH1D_ = (TH1D*) PassTH1D_->Clone(name_+"Total");		
		RatioTH1D_ = (TH1D*) PassTH1D_->Clone(name_);
		RatioTGraphAsymm_ = new TGraphAsymmErrors();
	}
	TH1Eff(const char* name, const char* title, unsigned int nBins, double startBin, double endBin)
	{
		name_=TString(name);
		title_=TString(title);
		PassTH1D_ = new TH1D(name_+"Pass", title_, nBins, startBin, endBin);
		PassTH1D_->Sumw2();
		TotalTH1D_ = (TH1D*) PassTH1D_->Clone(name_+"Total");	
		RatioTH1D_ = (TH1D*) PassTH1D_->Clone(name_);
		RatioTGraphAsymm_ = new TGraphAsymmErrors();
	}
	TH1Eff(const char* name, TDirectory* MainDirectory)
	{	
		name_=TString(name);
		MainDirectory->cd();
		TDirectory *effDir = (TDirectory*)MainDirectory->Get(name_);
		effDir->cd();
		RatioTH1D_ = (TH1D*) effDir->Get(name_);
		RatioTGraphAsymm_ = (TGraphAsymmErrors*) effDir->Get(name_+"Asymm");
	}
	void SetName(const char* name){
		name_=TString(name);
		PassTH1D_->SetName(name_+" Pass");
		TotalTH1D_->SetName(name_+" Total");
		RatioTH1D_->SetName(name_);
	}
	void SetTitle(const char* title){
		title_=TString(title);
		PassTH1D_->SetTitle(title_);
		TotalTH1D_->SetTitle(title_);
	}
	void Fill(Double_t x, Double_t Weight, bool passOrFail);
	void SaveEff(TDirectory* MainDirectory){ SaveEff(title_, MainDirectory); }
	void SaveEff(const char* title, TDirectory* MainDirectory, bool xlog=false, bool ylog=false);
	void OpenEff(const char* name, TDirectory* MainDirectory);
	effVec GetEff(double xValue){ return GetEff(xValue, false, false); }
	effVec GetEff(double xValue, bool asymm, bool verbose=false);
	~TH1Eff(){}
private:
	TH1D* PassTH1D_;
	TH1D* TotalTH1D_;
	TH1D* RatioTH1D_;
	TGraphAsymmErrors* RatioTGraphAsymm_;
	TString name_;
	TString title_;
};

void TH1Eff::Fill(Double_t x, Double_t Weight, bool passOrFail)
{
	if(passOrFail){
		PassTH1D_->Fill(x, Weight);
		TotalTH1D_->Fill(x, Weight);
	}else{
		TotalTH1D_->Fill(x, Weight);
	}
}

void TH1Eff::SaveEff(const char* title, TDirectory* MainDirectory, bool xlog, bool ylog)
{
	MainDirectory->cd();
	MainDirectory->mkdir(name_);
	TDirectory *effDir = (TDirectory*)MainDirectory->Get(name_);
	effDir->cd();

	RatioTH1D_->Divide(PassTH1D_, TotalTH1D_, 1, 1, "B");
	RatioTH1D_->SetName(name_);
	RatioTH1D_->SetTitle(TString("Simulation, L=3 fb^{-1}, #sqrt{s}=13 TeV ") + TString(title));
	RatioTH1D_->SetMarkerSize(2.0);
  	RatioTH1D_->UseCurrentStyle();

  	gROOT->SetBatch(true);	  
	TCanvas *c1 = new TCanvas(name_,title_,1);
	c1->cd();
	if (xlog) {
	  c1->SetLogx();
	  RatioTH1D_->GetXaxis()->SetRangeUser(0.001, 10*RatioTH1D_->GetBinLowEdge(RatioTH1D_->GetNbinsX()+1));
	}
	if (ylog) c1->SetLogy();
	if(RatioTH1D_->GetXaxis()->GetBinCenter(RatioTH1D_->GetXaxis()->GetNbins()) > 100) c1->SetLogx();
	RatioTH1D_->Draw("ColZ,Text,E");
	c1->SaveAs("Efficiencies/"+name_+".pdf");
	delete c1;
	gROOT->SetBatch(false);

	RatioTH1D_->Write();

	RatioTGraphAsymm_->Divide(PassTH1D_, TotalTH1D_, "cl=0.683 b(1,1) mode");
	RatioTGraphAsymm_->SetName(name_+"Asymm");
	RatioTGraphAsymm_->SetTitle(title);
	RatioTGraphAsymm_->Write();
}

effVec TH1Eff::GetEff(double xValue, bool asymm, bool verbose)
{
  	double result = 0;
  	double resultAsymm = 0;
  	double errUp_ = 0;
  	double errDown_ = 0;

	if (verbose) printf("Accessing efficiencies saved in %s\n",RatioTH1D_->GetName());

  	if(xValue < RatioTH1D_->GetXaxis()->GetXmin()){
	    std::cout<<"Warning xValue: "<<xValue<<" is smaller than minimum of histo: "<<RatioTH1D_->GetName()<<std::endl;
	    xValue= RatioTH1D_->GetXaxis()->GetXmin()+0.01;
	    //std::cout<<" Setting xValue to: "<<xValue<<std::endl;
	}
	else if(xValue > RatioTH1D_->GetXaxis()->GetXmax()){
	    std::cout<<"Warning xValue: "<<xValue<<" is bigger than maximum of histo: "<<RatioTH1D_->GetName()<<" which is: "<<RatioTH1D_->GetXaxis()->GetXmax();
	    xValue= RatioTH1D_->GetXaxis()->GetXmax()-0.01;
	    //std::cout<<" Setting xValue to: "<<xValue<<std::endl;
	}

	int nxBin = RatioTH1D_->GetXaxis()->FindBin(xValue);

	if(nxBin > RatioTH1D_->GetNbinsX()){
		std::cout <<"Problem getting Efficiencies!"<<std::endl;
		nxBin = RatioTH1D_->GetNbinsX();
	}

	result = RatioTH1D_->GetBinContent(nxBin);
	errUp_ = RatioTH1D_->GetBinErrorUp(nxBin);
	errDown_ = RatioTH1D_->GetBinErrorLow(nxBin);

	if(result<0.01){
		std::cout<<"Warning efficiency is: "<<result<<" is smaller than 0.01 for histo: "<<RatioTH1D_->GetName()<<std::endl;
		result =0.01;
	}
	if(result>1){
		std::cout<<"Warning efficiency is: "<<result<<" is bigger than 1 for histo: "<<RatioTH1D_->GetName()<<std::endl;
		result =0.99;
	}

	if(asymm && result>0.01){
		// empty bins in the end/beginning of the th's are removed when creating a tgraph..
	  	int nEmpty = 0;
	  	while(RatioTH1D_->GetBinContent(nEmpty+1) < 0.01){
	  		nEmpty++;
	  		if(nEmpty > RatioTH1D_->GetXaxis()->GetNbins()) break;
	  	}

		Double_t xValueAsymm;
		RatioTGraphAsymm_->GetPoint(nxBin-1, xValueAsymm, resultAsymm);
		errUp_ = RatioTGraphAsymm_->GetErrorYhigh(nxBin-1);
		errDown_ = RatioTGraphAsymm_->GetErrorYlow(nxBin-1);

		if (verbose) printf("xValue, xbin, eff: %3.0f, %d, %3.3f + %3.3f - %3.3f\n", xValue, nxBin, resultAsymm, errUp_, errDown_);

		//	if(std::abs(resultAsymm-result)>0.01) std::cout<<"Efficiencies of TGraph "<<name_<<" doesn't fit to THist!: "<<result<<"; "<<resultAsymm<<std::endl;
		effVec effVec_ = {resultAsymm, errUp_, errDown_};
        return effVec_;
	}
	if (verbose) printf("xValue, xbin, eff: %3.0f, %d, %3.3f + %3.3f - %3.3f\n", xValue, nxBin, result, errUp_, errDown_);

	effVec effVec_ = {result, errUp_, errDown_};
	return effVec_;
}

class TH2Eff
{
public:
	TH2Eff(){}
	TH2Eff(const char* name, const char* title, Int_t nbinsx, const double* xbins, Int_t nbinsy, const double* ybins)
	{
		name_=TString(name);
		title_=TString(title);
		nBinsy_=nbinsy;

		PassTH2D_ = new TH2D(name_+"Pass", title_, nbinsx, xbins, nbinsy, ybins);
		PassTH2D_->Sumw2();

		TotalTH2D_ = (TH2D*) PassTH2D_->Clone(name_+"Total");		
		RatioTH2D_ = (TH2D*) PassTH2D_->Clone(name_);

		for(int i = 0; i < nbinsy; ++i){
			PassTH1Dvec_.push_back(new TH1D(name_+"Pass_yBin"+std::to_string(i), title_, nbinsx, xbins));
			TotalTH1Dvec_.push_back(new TH1D(name_+"Total_yBin"+std::to_string(i), title_, nbinsx, xbins));
			RatioTGraphAsymmVec_.push_back(new TGraphAsymmErrors());
		}
	}
	TH2Eff(const char* name, const char* title, unsigned int nBinsx, double startBinx, double endBinx, unsigned int nBinsy, double startBiny, double endBiny)
	{
		name_=TString(name);
		title_=TString(title);
		nBinsy_=nBinsy;

		PassTH2D_ = new TH2D(name_+"Pass", title_, nBinsx, startBinx, endBinx, nBinsy, startBiny, endBiny);
		PassTH2D_->Sumw2();
		TotalTH2D_ = (TH2D*) PassTH2D_->Clone(name_+"Total");
		RatioTH2D_ = (TH2D*) PassTH2D_->Clone(name_);

		for(unsigned int i = 0; i < nBinsy; ++i){
			PassTH1Dvec_.push_back(new TH1D(name_+"Pass_yBin"+std::to_string(i+1), title_, nBinsx, startBinx, endBinx));
			TotalTH1Dvec_.push_back(new TH1D(name_+"Total_yBin"+std::to_string(i+1), title_, nBinsx, startBinx, endBinx));
			RatioTGraphAsymmVec_.push_back(new TGraphAsymmErrors());
		}
	}
	TH2Eff(const char* name, TDirectory* MainDirectory)
	{	
		name_=TString(name);
		MainDirectory->cd();
		TDirectory *effDir = (TDirectory*)MainDirectory->Get(name_);
		effDir->cd();
		RatioTH2D_ = (TH2D*) effDir->Get(name_);

		for(int i = 0; i < RatioTH2D_->GetYaxis()->GetNbins(); ++i){
			RatioTGraphAsymmVec_.push_back((TGraphAsymmErrors*) effDir->Get(name_+"_yBin"+std::to_string(i+1)));
		}	
		
	}
	void SetName(const char* name){
		name_=TString(name);
		PassTH2D_->SetName(name_+" Pass");
		TotalTH2D_->SetName(name_+" Total");
		RatioTH2D_->SetName(name_);
	}
	void SetTitle(const char* title){
		title_=TString(title);
		PassTH2D_->SetTitle(title_);
		TotalTH2D_->SetTitle(title_);
	}
	void Fill(Double_t x, Double_t y, Double_t Weight, bool passOrFail);
	void SaveEff(TDirectory* MainDirectory){ SaveEff(title_, MainDirectory); }
	void SaveEff(const char* title, TDirectory* MainDirectory, bool xlog=false, bool ylog=false);
	void OpenEff(const char* name, TDirectory* MainDirectory);
	effVec GetEff(double xValue, double yValue){ return GetEff(xValue, yValue, false); }
	effVec GetEff(double xValue, double yValue, bool asymm);
	~TH2Eff(){}
private:
	TH2D* PassTH2D_;
	TH2D* TotalTH2D_;
	TH2D* RatioTH2D_;

	std::vector<TH1D*> PassTH1Dvec_;
	std::vector<TH1D*> TotalTH1Dvec_;
	std::vector<TGraphAsymmErrors*> RatioTGraphAsymmVec_;

	TString name_;
	TString title_;

	int nBinsy_;
};

void TH2Eff::Fill(Double_t x, Double_t y, Double_t Weight, bool passOrFail)
{
	int nyBin = RatioTH2D_->GetYaxis()->FindBin(y);

	if(passOrFail){
		PassTH2D_->Fill(x, y, Weight);
		TotalTH2D_->Fill(x, y, Weight);
		
		if(nyBin > 0 && nyBin <= nBinsy_){
			PassTH1Dvec_.at(nyBin-1)->Fill(x, Weight);
			TotalTH1Dvec_.at(nyBin-1)->Fill(x, Weight);
		}

	}else{
		TotalTH2D_->Fill(x, y, Weight);

		if(nyBin > 0 && nyBin <= nBinsy_){
			TotalTH1Dvec_.at(nyBin-1)->Fill(x, Weight);
		}
	}
}

void TH2Eff::SaveEff(const char* title, TDirectory* MainDirectory, bool xlog, bool ylog)
{
	MainDirectory->cd();
	MainDirectory->mkdir(name_);
	TDirectory *effDir = (TDirectory*)MainDirectory->Get(name_);
	effDir->cd();

	RatioTH2D_->Divide(PassTH2D_, TotalTH2D_, 1, 1, "B");
	RatioTH2D_->SetName(name_);
	RatioTH2D_->SetTitle(TString("Simulation, L=3 fb^{-1}, #sqrt{s}=13 TeV ") + TString(title));
	RatioTH2D_->SetMarkerSize(2.0);
  	RatioTH2D_->UseCurrentStyle();

  	gROOT->SetBatch(true);	  
	TCanvas *c1 = new TCanvas(name_,title_,1);
	c1->cd();
	if (xlog) c1->SetLogx();
	if (ylog) c1->SetLogy();
	if(RatioTH2D_->GetXaxis()->GetBinCenter(RatioTH2D_->GetXaxis()->GetNbins()) > 100) c1->SetLogx();
	if(RatioTH2D_->GetYaxis()->GetBinCenter(RatioTH2D_->GetYaxis()->GetNbins()) > 100) c1->SetLogy();
	RatioTH2D_->GetZaxis()->SetRangeUser(0., 1.5);
	RatioTH2D_->Draw("ColZ,Text,E");
	c1->SaveAs("Efficiencies/"+name_+".pdf");
	delete c1;
	gROOT->SetBatch(false);

	RatioTH2D_->Write();

	for(int i = 0; i < nBinsy_; ++i){
		RatioTGraphAsymmVec_.at(i)->Divide(PassTH1Dvec_.at(i), TotalTH1Dvec_.at(i), "cl=0.683 b(1,1) mode");
		RatioTGraphAsymmVec_.at(i)->SetName(name_+"_yBin"+std::to_string(i+1));
		RatioTGraphAsymmVec_.at(i)->SetTitle(title);
		RatioTGraphAsymmVec_.at(i)->GetYaxis()->SetTitle("");
		RatioTGraphAsymmVec_.at(i)->Write();
	}
}

effVec TH2Eff::GetEff(double xValue, double yValue, bool asymm)
{
  double result = 0;
  double errUp_ = 0;
  double errDown_ = 0;
  double resultAsymm = 0;
  double errUpAsymm_ = 0;
  double errDownAsymm_ = 0;

  if(xValue < RatioTH2D_->GetXaxis()->GetXmin() )
  {
    std::cout<<"Warning xValue: "<<xValue<<" is smaller than minimum of histo: "<<RatioTH2D_->GetName()<<std::endl;
    xValue= RatioTH2D_->GetXaxis()->GetXmin()+0.01;
    //std::cout<<" Setting xValue to: "<<xValue<<std::endl;
  }
  else if(xValue > RatioTH2D_->GetXaxis()->GetXmax() )
  {
    std::cout<<"Warning xValue: "<<xValue<<" is bigger than maximum of histo: "<<RatioTH2D_->GetName()<<" which is: "<<RatioTH2D_->GetXaxis()->GetXmax()<<std::endl;
    xValue= RatioTH2D_->GetXaxis()->GetXmax()-0.01;
    //std::cout<<" Setting xValue to: "<<xValue<<std::endl;
  }
  
  if(yValue < RatioTH2D_->GetYaxis()->GetXmin() )
  {
    std::cout<<"Warning yValue: "<<yValue<<" is smaller than minimum of histo: "<<RatioTH2D_->GetName()<<std::endl;
    yValue= RatioTH2D_->GetYaxis()->GetXmin()+0.01;
    //std::cout<<" Setting yValue to: "<<yValue<<std::endl;
  }
  else if(yValue > RatioTH2D_->GetYaxis()->GetXmax() )
  {
    std::cout<<"Warning yValue: "<<yValue<<" is bigger than maximum of histo: "<<RatioTH2D_->GetName()<<std::endl;
    yValue= RatioTH2D_->GetYaxis()->GetXmax()-0.01;
    //std::cout<<" Setting yValue to: "<<yValue<<std::endl;
  }

  int nxBin = RatioTH2D_->GetXaxis()->FindBin(xValue);
  int nyBin = RatioTH2D_->GetYaxis()->FindBin(yValue);

  if(nxBin > RatioTH2D_->GetNbinsX() || nyBin > RatioTH2D_->GetNbinsY()) std::cout<<"Problem in getting Efficiencies!"<<std::endl;
  if(nxBin > RatioTH2D_->GetNbinsX()) nxBin = RatioTH2D_->GetNbinsX();
  if(nyBin > RatioTH2D_->GetNbinsY()) nyBin = RatioTH2D_->GetNbinsY();

  result = RatioTH2D_->GetBinContent(nxBin, nyBin);
  errUp_ = RatioTH2D_->GetBinErrorUp(nxBin, nyBin);
  errDown_ = RatioTH2D_->GetBinErrorLow(nxBin, nyBin);

  if(result<0.01)
  {
    std::cout<<"Warning efficiency is: "<<result<<" is smaller than 0.01 for histo: "<<RatioTH2D_->GetName()<<std::endl;
    effVec effVec_ = {0.01, 0.99, 0.01};
    return effVec_; 
  }
  if(result>1)
  {
    std::cout<<"Warning efficiency is: "<<result<<" is bigger than 1 for histo: "<<RatioTH2D_->GetName()<<std::endl;
    result =0.99;
  }

  if(asymm && result>0.01){
  	// empty bins in the end/beginning of the th's are removed when creating a tgraph.. WORKAROUND:
  	int nEmpty = 0;
  	while(RatioTH2D_->GetBinContent(nEmpty+1, nyBin) < 0.01){
  		nEmpty++;
  		if(nEmpty > RatioTH2D_->GetXaxis()->GetNbins()) break;
  	}

  	Double_t xValueAsymm;
  	Double_t yValueAsymm = 1;
  	for(int i = 0; i < RatioTH2D_->GetNbinsX(); ++i){
  		RatioTGraphAsymmVec_.at(nyBin-1)->GetPoint(i, xValueAsymm, yValueAsymm);
  		if(yValueAsymm < 0.01) nEmpty--;
  		else break;
  	}

  	RatioTGraphAsymmVec_.at(nyBin-1)->GetPoint(nxBin-1-nEmpty, xValueAsymm, resultAsymm);
  	errUpAsymm_ = RatioTGraphAsymmVec_.at(nyBin-1)->GetErrorYhigh(nxBin-1-nEmpty);
	errDownAsymm_ = RatioTGraphAsymmVec_.at(nyBin-1)->GetErrorYlow(nxBin-1-nEmpty);

	if(std::abs(resultAsymm-result)>0.01){
		std::cout<<"Efficiencies of TGraph "<<name_<<" doens't fit to THist!: "<<result<<"; "<<resultAsymm<<std::endl;
		effVec effVec_ = {result, errUp_, errDown_};
  		return effVec_;
	}

	effVec effVec_ = {resultAsymm, errUpAsymm_, errDownAsymm_};
    return effVec_;
  }

  effVec effVec_ = {result, errUp_, errDown_};
  return effVec_;
}












/// outdated! not to be used anymore!


class TH1Deff
{
public:
	TH1Deff(){}
	TH1Deff(const char* name, const char* title, Int_t nbinsx, const double* xbins)
	{
		RefTH1D_ = new 	TH1D(name, title, nbinsx, xbins);
		RefTH1D_->Sumw2();
		name_=name;
		title_=title;
	}
	TH1Deff(const char* name, const char* title, unsigned int nBins, double startBin, double endBin)
	{
		RefTH1D_ = new 	TH1D(name, title, nBins, startBin,endBin);
		RefTH1D_->Sumw2();
		name_=name;
		title_=title;
	}
	TH1D* Clone(){return RefTH1D_;}
	void SetName(const char* title){RefTH1D_->SetName(title); }
	void SetTitle(const char* title){RefTH1D_->SetTitle(title);}
	void Fill(Double_t x,Double_t Weight,bool passOrFail);
	TGraphAsymmErrors* GetEfficiency();
	TH1D* GetSanityCheckTH1D(){return RefTH1D_;}
	void saveResults(TDirectory* MainDirectory);
	~TH1Deff(){}
private:
	TH1D* RefTH1D_;
	vector<Double_t> weights_;
	vector<TH1D*> TH1DFail_, TH1DPass_;
	const char* name_;
	const char* title_;
};

class TH2Deff
{
public:
	TH2Deff(){}
	TH2Deff(const char* name, const char* title, Int_t nbinsx, const Double_t* xbins, Int_t nbinsy, const Double_t* ybins)
	{
		RefTH2D_ = new 	TH2D(name, title, nbinsx, xbins,nbinsy,ybins);
		RefTH2D_->Sumw2();
		nbinsx_=nbinsx; xbins_=xbins;
		nbinsy_=nbinsy; ybins_=ybins;
		name_=name;
		title_=title;
	}
	TH2D* Clone(){return RefTH2D_;}
	void SetName(const char* title){RefTH2D_->SetName(title);}
	void Fill(Double_t x, Double_t y, Double_t Weight,bool passOrFail);
	std::vector<TGraphAsymmErrors*> GetEfficiency();
	TH2D* GetSanityCheckTH2D(){return RefTH2D_;}
	void saveResults(TDirectory* MainDirectory);
	~TH2Deff(){}
private:
	TH2D* RefTH2D_;
	vector<Double_t> weights_;
	vector<TH2D*> TH2DFail_, TH2DPass_;
	Int_t nbinsx_, nbinsy_;
	const Double_t* xbins_;
	const Double_t* ybins_;
	const char* name_;
	const char* title_;
};
class Efficiency : public SearchBins
{
public:
	Efficiency(){}
	Efficiency(const char* name, const char* title);

	void Fill(double HT, double MHT, int NJets, int BTags, double Weight, bool passOrFail);

	void saveResults(TDirectory* MainDirectory);
	~Efficiency(){}
private:	
	const char* name_;
	const char* title_;
	TH1Deff* TH1DSearchBins_;
	std::vector<TH1Deff*> TH1DSearchBinsSplit_;
	unsigned int splitAfter_;
	
};
class SearchBinEventCount : public SearchBins
{
public:
	SearchBinEventCount(){}
	SearchBinEventCount(const char*);
	void Fill(double HT, double MHT, int NJets, int BTags, double Weight);
	void saveResults(TDirectory* MainDirectory);
	TH1D* getFullTH1D(){return fullTH1D_;}
	~SearchBinEventCount(){}
private:
	TH1D* fullTH1D_;
	std::vector<TH1D*> splitTH1D_;
	unsigned int splitAfter_;
	const char* name_;
};


void TH1Deff::Fill(Double_t x,Double_t Weight, bool passOrFail)
{
	int matched=-1;
	for(unsigned int i=0; i < weights_.size();i++)
	{
		if(std::abs(weights_[i]-Weight)/Weight<0.0001 ) matched=i;
	}
	if(matched==-1)
	{
		TH1D* tempPass = (TH1D*) RefTH1D_->Clone();
		TH1D* tempFail = (TH1D*) RefTH1D_->Clone();
		TH1DPass_.push_back(tempPass );
		TH1DFail_.push_back(tempFail );

		if(passOrFail)
		{
			TH1DPass_[weights_.size()]->Fill(x);
		}
		else TH1DFail_[weights_.size()]->Fill(x);
		weights_.push_back(Weight);
// 		std::cout<<"Weight: "<<Weight<<"added"<<std::endl;
	}
	else
	{
		if(passOrFail) TH1DPass_[matched]->Fill(x);
		else TH1DFail_[matched]->Fill(x);
	}
}
TGraphAsymmErrors* TH1Deff::GetEfficiency()
{
	TList* myList = new TList(); 
	// compute th1 for sanity check
	TH1D* sumRef = (TH1D*) RefTH1D_->Clone();
	sumRef->Sumw2();
	for(unsigned int i=0; i< TH1DFail_.size();i++)
	{
		sumRef->Add(TH1DPass_[i],weights_[i]);
		RefTH1D_->Add(TH1DPass_[i],weights_[i]);
		sumRef->Add(TH1DFail_[i],weights_[i]);
	}
	RefTH1D_->Divide(RefTH1D_,sumRef,1,1,"B");
	for(unsigned int i=0; i<weights_.size();i++)
	{
		TH1DFail_[i]->Sumw2();
		TH1D *sum = (TH1D*)TH1DFail_[i]->Clone();
		TH1DPass_[i]->Sumw2();
		
		sum->Add(TH1DPass_[i]);
		TEfficiency* myEff = new TEfficiency(*TH1DPass_[i],*sum);
// 		std::cout<<"Eff["<<i<<"]: "<<myEff->GetEfficiency(14)<<" +"<<myEff->GetEfficiencyErrorUp(14)<<" - "<<myEff->GetEfficiencyErrorLow(14)<<" passed: "<<myEff->GetPassedHistogram()->GetBinContent(14)<<" totalN: "<<myEff->GetTotalHistogram()->GetBinContent(14)<<" Weight: "<<weights_[i]<<" xCenter: "<<myEff->GetPassedHistogram()->GetBinCenter(14)<<"\n";
// 		myEff->SetWeight(weights_[i]);
		myList->Add(myEff);
	}
	const Int_t size=weights_.size();
	Double_t weightArray[size];
	for(int i=0; i<size;i++)weightArray[i]=weights_[i];
	
	TGraphAsymmErrors* result= TEfficiency::Combine ((TCollection*)&*myList,(Option_t*)"kBUniform",(Int_t)weights_.size(),weightArray);
// 	std::cout<<"Total Eff: "<<result->GetY()[13]<<" + "<<result->GetEYhigh()[13]<<" - "<<result->GetErrorYlow(13)<<" x value: "<<result->GetX()[13]<<"\n";
// 	std::cout<<"-----------------------------------------------------------------Name: "<<name_ <<"\n";
	result->SetName(name_);
	result->SetTitle(title_);
	return result;
}

void TH1Deff::saveResults(TDirectory* MainDirectory)
{
// 	MainDirectory->mkdir(name_);
// 	std::cout<<"name: "<<name_<<std::endl;
// 	TDirectory *dir = (TDirectory*)MainDirectory->Get(name_);
// 	dir->cd();
	MainDirectory->cd();
	TGraphAsymmErrors* result = GetEfficiency();
	result->SetName(RefTH1D_->GetName());
	result->SetTitle(RefTH1D_->GetTitle());
	result->Write();
	TString tempName = RefTH1D_->GetName();
	tempName+="_SanityCheck";
	MainDirectory->mkdir(tempName);
	// 	std::cout<<"name: "<<name_<<std::endl;
	TDirectory *sanityDir = (TDirectory*)MainDirectory->Get(tempName);
	sanityDir->cd();
	RefTH1D_->Write();
	
}


void TH2Deff::Fill(Double_t x, Double_t y, Double_t Weight, bool passOrFail)
{
	int matched=-1;
	for(unsigned int i=0; i < weights_.size();i++)
	{
		if(std::abs(weights_[i]-Weight)/Weight<0.0001 ) matched=i;
	}
	if(matched==-1)
	{
		TH2D* tempPass = (TH2D*) RefTH2D_->Clone();
		tempPass->Sumw2();
		TH2D* tempFail = (TH2D*) RefTH2D_->Clone();
		tempFail->Sumw2();
		TH2DPass_.push_back(tempPass );
		TH2DFail_.push_back(tempFail );
		
		if(passOrFail)	TH2DPass_[weights_.size()]->Fill(x,y);
		else TH2DFail_[weights_.size()]->Fill(x,y);
		weights_.push_back(Weight);
// 		std::cout<<"Weight: "<<Weight<<" added"<<std::endl;
	}
	else
	{
		if(passOrFail) TH2DPass_[matched]->Fill(x,y);
		else TH2DFail_[matched]->Fill(x,y);
	}
}
std::vector<TGraphAsymmErrors*> TH2Deff::GetEfficiency()
{ 
	const Int_t nBinsY = RefTH2D_->GetNbinsY();
	std::cout<<"Number of binsY: "<<nBinsY<<std::endl;
	std::vector<std::vector<TH1D*> > th1PassVec, th1FailVec;
	std::vector<TList*> myLists;
	// reference sum th2f
	TH2D* sumTH2ftemp = (TH2D*) RefTH2D_->Clone();
	sumTH2ftemp->Sumw2();

	for(unsigned int i=0; i < TH2DPass_.size();i++)
	{
		RefTH2D_->Add(TH2DPass_[i],weights_[i]);
		sumTH2ftemp->Add(TH2DPass_[i],weights_[i]);
		sumTH2ftemp->Add(TH2DFail_[i],weights_[i]);
	}
	RefTH2D_->Divide(RefTH2D_,sumTH2ftemp,1,1,"B");
	for(int i=0; i<nBinsY;i++)
	{
		myLists.push_back(new TList());
		TString temp1 ( Form ("%d", i));
		temp1= (TString)name_+"Bin"+temp1;
		TString temp2 (Form ("_%f.0-%f.0",ybins_[i],ybins_[i+1]));
		temp2= (TString)title_+temp2;
		std::vector<TH1D*> tt;
		th1PassVec.push_back(tt);
		th1FailVec.push_back(tt);
	for(unsigned int ii=0; ii<weights_.size();ii++)
	{
		TString temp3 ( Form ("%d", ii));
		temp1= temp1+"_"+temp3;
		temp3=temp1+"pass";
		th1PassVec[i].push_back(new TH1D(temp3,temp2,nbinsx_, xbins_));
		temp3=temp1+"fail";
		th1FailVec[i].push_back(new TH1D(temp3,temp2,nbinsx_, xbins_));
		for(int iii=0; iii<nbinsx_+1;iii++)
		{
			th1PassVec[i][ii]->SetBinContent(iii,TH2DPass_[ii]->GetBinContent(iii,i+1) );
// 			std::cout<<"P:BinContent["<<i<<"]["<<ii<<"]["<<iii<<"]: "<<TH2DPass_[ii]->GetBinContent(iii,i+1)<<" +- "<<TH2DPass_[ii]->GetBinError(iii,i+1)<<std::endl;
			th1PassVec[i][ii]->SetBinError(iii,TH2DPass_[ii]->GetBinError(iii,i+1) );
			th1FailVec[i][ii]->SetBinContent(iii,TH2DFail_[ii]->GetBinContent(iii,i+1) );
// 			std::cout<<"F:BinContent["<<i<<"]["<<ii<<"]["<<iii<<"]: "<<TH2DFail_[ii]->GetBinContent(iii,i)<<" +- "<<TH2DFail_[ii]->GetBinError(iii,i+1)<<std::endl;
			th1FailVec[i][ii]->SetBinError(iii,TH2DFail_[ii]->GetBinError(iii,i+1) );
		}
		TH1D *sum = (TH1D*)th1FailVec[i][ii]->Clone();
// 		sum->SetTitle()
		sum->Add(th1PassVec[i][ii]);
		TEfficiency* myEff = new TEfficiency(*th1PassVec[i][ii],*sum);
		// 		myEff->SetWeight(weights_[i]);
		myLists[i]->Add(myEff);
	}
	}
	const Int_t size=weights_.size();
	Double_t weightArray[size];
	for(int i=0; i<size;i++)weightArray[i]=weights_[i];
	std::vector<TGraphAsymmErrors*> result; //= TEfficiency::Combine ((TCollection*)&*myList,(Option_t*)"kFCP",(Int_t)weights_.size(),weightArray);
	for(int i=0; i<nBinsY;i++)
	{
		result.push_back(TEfficiency::Combine ((TCollection*)&*myLists[i],(Option_t*)"kBUniform",(Int_t)weights_.size(),weightArray) );
	}
	
	return result;
}

void TH2Deff::saveResults(TDirectory* MainDirectory)
{
	MainDirectory->mkdir(name_);
// 	std::cout<<"name: "<<name_<<std::endl;
	TDirectory *dir = (TDirectory*)MainDirectory->Get(name_);
	dir->cd();
	RefTH2D_->SetName(name_);
	RefTH2D_->Write();
	std::vector<TGraphAsymmErrors*> result = GetEfficiency();
	for(unsigned int i=0; i<result.size();i++) 
	{
		TString temp2 (Form ("_%d-%d",(int)ybins_[i],(int)ybins_[i+1]));
		temp2= (TString)title_+temp2;
		TString temp3 ( Form ("%d", i));
		temp3= (TString)name_+temp3;
		result[i]->SetName(temp3);
		result[i]->SetTitle(temp2);
		result[i]->Write();
	}
	dir->mkdir("SanityCheck");
	// 	std::cout<<"name: "<<name_<<std::endl;
	TDirectory *sanityDir = (TDirectory*)dir->Get("SanityCheck");
	sanityDir->cd();
	RefTH2D_->Write();
}


SearchBinEventCount::SearchBinEventCount(const char* name)
{
	fullTH1D_ = new 	TH1D(name, name, bins_.size(),0,bins_.size()+1);
	fullTH1D_->Sumw2();
	name_=name;
/*	splitAfter_=18;
	unsigned int plotsNumber= bins_.size()/splitAfter_;
	// 	std::cout<<"Efficiency::Efficiency plotsNumber="<<plotsNumber<<" number of bins: "<<bins_.size()<<std::endl;
	if(plotsNumber * splitAfter_<bins_.size() )
	{
		
		plotsNumber++;
		for(unsigned int i=0; i < plotsNumber;i++)
		{
			
			TString temp2 (Form ("_%d",(int)i+1));
			TString temp1 = name;
			temp1+=temp2;
			temp2 = name + temp2;
			const char* name1=temp1;
			const char* title1=temp2;
			
			if(i+1==plotsNumber)
			{
				// 				std::cout<<"titlelast["<<i<<"]: "<<temp1<<std::endl;
				unsigned int tempBins = bins_.size() - plotsNumber * splitAfter_;
				splitTH1D_.push_back( new TH1D (name1,title1,tempBins,0,tempBins+1) );
				splitTH1D_[i]->Sumw2();
				continue;
			}
			// 			std::cout<<"title["<<i<<"]: "<<temp1<<std::endl;
			splitTH1D_.push_back( new TH1D (name1,title1,splitAfter_,0,splitAfter_+1) );
			splitTH1D_[i]->Sumw2();
		}
	}
	else
	{
		for(unsigned int i=0; i < plotsNumber;i++)
		{
			
			TString temp2 (Form ("_%d",(int)i+1));
			TString temp1 = name;
			temp1+=temp2;

			const char* name1=temp1;
			TH1D* tempeff2 = new  TH1D (name1,name1,splitAfter_,0,splitAfter_+1);
			splitTH1D_.push_back( tempeff2 );
			splitTH1D_[i]->Sumw2();
		}
	}
*/}


void SearchBinEventCount::Fill(double HT, double MHT, int NJets, int BTags, double Weight)
{
	double bin = GetBinNumber(HT,MHT,NJets,BTags);
	
	if(bin<bins_.size()+2) 
	{
		fullTH1D_->Fill(bin-0.01, Weight);
	/*	unsigned int splitHist=0;
		// 	std::cout<<"bin before split: "<<bin<<std::endl;
		for(int ii=0;bin>splitAfter_;ii++)
		{
			splitHist++;
			bin = bin-splitAfter_;
		}
		// 		if(splitHist==3)std::cout<<"BinForSplit: "<<bin<<" with splitHistNumber "<<splitHist<<" and TH1DSearchBinsSplit_.size(): "<<TH1DSearchBinsSplit_.size()<<std::endl;
		
		splitTH1D_[splitHist]->Fill(bin-0.1, Weight);
	*/}
}
void SearchBinEventCount::saveResults(TDirectory* MainDirectory)
{
	MainDirectory->mkdir(name_);
	// 	std::cout<<"name: "<<name_<<std::endl;
	TDirectory *dir = (TDirectory*)MainDirectory->Get(name_);
	dir->cd();
	fullTH1D_->Write();
/*	for(unsigned int i=0; i<splitTH1D_.size();i++) 
	{
		TString temp2 (Form ("_%d",(int)i+1));
		TString temp1 = name_;
		temp1+=temp2;
		temp2 = name_ + temp2;
		const char* name1=temp1;
		const char* title1=temp2;
		splitTH1D_[i]->SetTitle(title1);
		splitTH1D_[i]->SetName(name1);
		splitTH1D_[i]->Write();
	}
*/}
