#ifndef LEPTON_ACCEPTANCE_H
#define LEPTON_ACCEPTANCE_H

#include <iostream>
#include <exception>
#include <vector>

#include "TFile.h"
#include "TH2.h"
#include "TString.h"


// Access the lepton acceptance and related information
// The acceptance is determined in bins of MHT and N(jets)
//
// Author: Matthias Schroeder
//         matthias.schroeder@AT@desy.de
//         November 2013
class LeptonAcceptance {
public:
  // Definition of the lepton acceptance
  static double muonPtMin() { return 10.; }
  static double muonEtaMax() { return 2.4; }
  static double electronPtMin() { return 10.; }
  static double electronEtaMax() { return 5.0; }

  // MHT bins for acceptance parametrisation
  static std::vector<double> mhtBinEdges() {
    std::vector<double> bins;
    bins.push_back(200.);
    bins.push_back(600.);
    bins.push_back(1600.);

    return bins;
  }

  // N(jets) bins for acceptance parametrisation
  static std::vector<double> nJetsBinEdges() {
    std::vector<double> bins;
    bins.push_back(3);
    bins.push_back(4);
    bins.push_back(6);
    bins.push_back(7);
    bins.push_back(14);

    return bins;
  }

  // Labels for histogram names
  static TString nameMuonAcc() { return "MuonAcc"; }
  static TString nameElecAcc() { return "ElecAcc"; }

  // Constructor.
  LeptonAcceptance(const TString &fileName, const TString &histName);
  ~LeptonAcceptance();

  // Access the lepton acceptance. Returns the probability to find
  // a lepton within acceptance for an event with the specified
  // MHT and N(jets)
  double operator()(double mht, unsigned int nJets) const;
  

private:
  TH2* acc_;
  double mhtMin_;
  double mhtMax_;
  double nJetsMin_;
  double nJetsMax_;
};



LeptonAcceptance::LeptonAcceptance(const TString &fileName, const TString &histName) {
  TFile file(fileName,"READ");
  TH2* h = NULL;
  file.GetObject(histName,h);
  if( h == NULL ) {
    std::cerr << "\n\nERROR reading acceptance map '" << histName << "' from file\n\n" << std::endl;
    throw std::exception();
  } else {
    h->SetDirectory(0);
    acc_ = h;
    mhtMin_ = h->GetXaxis()->GetBinLowEdge(1);
    mhtMax_ = h->GetXaxis()->GetBinUpEdge(h->GetNbinsX());
    nJetsMin_ = h->GetYaxis()->GetBinLowEdge(1);
    nJetsMax_ = h->GetYaxis()->GetBinUpEdge(h->GetNbinsY());
  }
}


LeptonAcceptance::~LeptonAcceptance() {
  delete acc_;
}


double LeptonAcceptance::operator()(double mht, unsigned int nJets) const {
  double mhtTmp = mht;
  if( mhtTmp < mhtMin_ ) mhtTmp = mhtMin_ + 0.01;
  if( mhtTmp > mhtMax_ ) mhtTmp = mhtMax_ - 0.01;
  double nJetsTmp = nJets;
  if( nJetsTmp < nJetsMin_ ) nJetsTmp = nJetsMin_ + 0.01;
  if( nJetsTmp > nJetsMax_ ) nJetsTmp = nJetsMax_ - 0.01;

  return acc_->GetBinContent(acc_->FindBin(mhtTmp,nJetsTmp));
}

#endif
