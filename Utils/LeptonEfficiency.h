#ifndef LEPTON_EFFICIENCY_H
#define LEPTON_EFFICIENCY_H

#include <iostream>
#include <exception>
#include <vector>

#include "TFile.h"
#include "TH2.h"
#include "TString.h"


// Access the lepton reconstruction or isolation efficiencies
// and related information. The efficiencies are determined
// in bins of HT, MHT, and N(jets).
//
// Author: Matthias Schroeder
//         matthias.schroeder@AT@desy.de
//         November 2013
class LeptonEfficiency {
public:
  // N(jets) binnning for efficiency parametrisation
  static unsigned int numNJetBins() { return 3; }
  static TString nJetBinId(unsigned int bin);
  static unsigned int nJetBin(int nJets);

  // MHT bins for efficiency parametrisation
  static std::vector<double> mhtBinEdges(unsigned int nJetBin);

  // HT bins for efficiency parametrisation
  static std::vector<double> htBinEdges(unsigned int nJetBin);

  // Labels for histogram names  
  static TString nameMuonRecoEff() { return "MuonRecoEff"; }
  static TString nameMuonRecoEff(unsigned int nJetBin) { return nameMuonRecoEff()+nJetBinId(nJetBin); }
  static TString nameMuonIsoEff() { return "MuonIsoEff"; }
  static TString nameMuonIsoEff(unsigned int nJetBin) { return nameMuonIsoEff()+nJetBinId(nJetBin); }
  static TString nameElecRecoEff() { return "ElecRecoEff"; }
  static TString nameElecRecoEff(unsigned int nJetBin) { return nameElecRecoEff()+nJetBinId(nJetBin); }
  static TString nameElecIsoEff() { return "ElecIsoEff"; }
  static TString nameElecIsoEff(unsigned int nJetBin) { return nameElecIsoEff()+nJetBinId(nJetBin); }


  // Constructor. 'histNameBase' is appended by the appropriate 
  // suffix for the N(jets) bins.
  LeptonEfficiency(const TString &fileName, const TString &histNameBase);
  ~LeptonEfficiency();

  // Return the efficiency
  double operator()(double ht, double mht, unsigned int nJets) const;
  

private:
  std::vector<TH2*> effs_;
  double nJetsMin_;
  double htMin_;
  double htMax_;
  double mhtMin_;
  double mhtMax_;

  unsigned int getNJetIdx(unsigned int nJets) const;
};



LeptonEfficiency::LeptonEfficiency(const TString &fileName, const TString &histNameBase) {
  TFile file(fileName,"READ");
  for(unsigned int i = 0; i < numNJetBins(); ++i) {
    const TString histName(histNameBase+nJetBinId(i));
    TH2* h = NULL;
    file.GetObject(histName,h);
    if( h == NULL ) {
      std::cerr << "\n\nERROR reading efficiency map '" << histName << "' from file\n\n" << std::endl;
      throw std::exception();
    } else {
      h->SetDirectory(0);
      effs_.push_back(h);
      if( i == 0 ) {
	nJetsMin_ = 3;
	htMin_ = h->GetXaxis()->GetBinLowEdge(1);
	htMax_ = h->GetXaxis()->GetBinUpEdge(h->GetNbinsX());
	mhtMin_ = h->GetYaxis()->GetBinLowEdge(1);
	mhtMax_ = h->GetYaxis()->GetBinUpEdge(h->GetNbinsY());
      }
    }
  }
}


LeptonEfficiency::~LeptonEfficiency() {
  for(std::vector<TH2*>::iterator it = effs_.begin();
      it != effs_.end(); ++it) {
    delete *it;
  }
}


double LeptonEfficiency::operator()(double ht, double mht, unsigned int nJets) const {
  double htTmp = ht;
  if( htTmp < htMin_ ) htTmp = htMin_ + 0.01;
  if( htTmp > htMax_ ) htTmp = htMax_ - 0.01;
  double mhtTmp = mht;
  if( mhtTmp < mhtMin_ ) mhtTmp = mhtMin_ + 0.01;
  if( mhtTmp > mhtMax_ ) mhtTmp = mhtMax_ - 0.01;
  double nJetsTmp = nJets;
  if( nJetsTmp < nJetsMin_ ) nJetsTmp = nJetsMin_ + 0.01;

  TH2* h = effs_.at(getNJetIdx(nJetsTmp));

  return h->GetBinContent(h->FindBin(htTmp,mhtTmp));
}


TString LeptonEfficiency::nJetBinId(unsigned int bin) {
  TString id("");
  if(      bin == 0 ) id = "NJets3-5";
  else if( bin == 1 ) id = "NJets6-7";
  else if( bin == 2 ) id = "NJets8-Inf";
  
  return id;
}


unsigned int LeptonEfficiency::nJetBin(int nJets) {
  unsigned int bin = 2;
  if( nJets >= 3 ) {
    if(      nJets <= 5 ) bin = 0;
    else if( nJets <= 7 ) bin = 1;
  } else {
    std::cerr << "ERROR jet multiplicity nJets = " << nJets << " out of lepton-efficiency binning" << std::endl;
    throw std::exception();
  }
  
  return bin;
}


unsigned int LeptonEfficiency::getNJetIdx(unsigned int nJets) const {
  unsigned idx = 0;
  if( nJets >= 3 && nJets <= 5 ) {
    idx = 0;
  } else if( nJets >=6 && nJets <=7 ) {
    idx = 1;
  } else if( nJets >= 8 ) {
    idx = 2;
  } else {
    std::cerr << "\n\nERROR trying to obtain lepton efficiency for nJets < 3\n\n" << std::endl;
    throw std::exception();
  }

  return idx;
}


std::vector<double> LeptonEfficiency::htBinEdges(unsigned int nJetBin) {
  std::vector<double> bins;
  if( nJetBin == 0 ) {
    // NJets 3 - 5
    bins.push_back(500);
    bins.push_back(800);
    bins.push_back(1000);
    bins.push_back(1250);
    bins.push_back(2500);
  } else if( nJetBin == 1 ) {
    // NJets 6 - 7
    bins.push_back(500);
    bins.push_back(800);
    bins.push_back(1000);
    bins.push_back(1250);
    bins.push_back(1500);
    bins.push_back(2500);
  } else if( nJetBin == 2 ) {
    // NJets > 7
    bins.push_back(500);
    bins.push_back(800);
    bins.push_back(1000);
    bins.push_back(1250);
    bins.push_back(1500);
    bins.push_back(2500);
  }

  return bins;
}


std::vector<double> LeptonEfficiency::mhtBinEdges(unsigned int nJetBin) {
  std::vector<double> bins;
  if( nJetBin == 0 ) {
    // NJets 3 - 5
    bins.push_back(200);
    bins.push_back(300);
    bins.push_back(450);
    bins.push_back(600);
    bins.push_back(2500);
  } else if( nJetBin ==  1 ) {
    // NJets 6 - 7
    bins.push_back(200);
    bins.push_back(300);
    bins.push_back(450);
    bins.push_back(2500);
  } else if( nJetBin == 2 ) {
    // NJets > 7
    bins.push_back(200);
    bins.push_back(2500);
  }

  return bins;
}

#endif
