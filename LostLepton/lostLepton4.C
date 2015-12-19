// Perform lost-lepton prediction on data
//
// Author: Matthias Schroeder
//         matthias.schroeder@AT@desy.de
//         November 2013

#include <algorithm>
#include <cmath>
#include <iostream>

#include "TFile.h"
#include "TH1.h"
#include "TH1D.h"
#include "TString.h"
#include "TVector2.h"

#include "../Utils/Event.h"
#include "../Utils/LeptonAcceptance.h"
#include "../Utils/LeptonEfficiency.h"
#include "../Utils/Sample.h"
#include "../Utils/Selection.h"


// === Declaration of Auxiliary Functions ==============================

// The probability for the muon to be out-of acceptance,
// not reconstructed, or not isolated
double lostLeptonProb(double acc, double reco, double iso);

// Correction factor to account for the fact that the muon
// control sample itself contains only events with well-reconstructed,
// isolated muons inside the acceptance.
double controlSampleCorr(double acc, double reco, double iso);


// === Main Function ===================================================
void lostLepton4(unsigned int id = 1,
		 const TString &effName = "../data/LLEff-SUS-13-012.root",
		 int nEvts = -1) {

  // --- Declare the Output Histograms ---------------------------------
  const TString title = "Lost-Lepton Prediction";

  TH1* hHtPred = new TH1D("hHtPred",title+";H_{T} [GeV]",25,500,3000);
  hHtPred->Sumw2();
  TH1* hMhtPred = new TH1D("hMhtPred",title+";#slash{H}_{T} [GeV]",20,200,1200);
  hMhtPred->Sumw2();
  TH1* hNJetsPred = new TH1D("hNJetsPred",title+";N(jets)",9,3,12);
  hNJetsPred->Sumw2();

  // Event yields in the different RA2 search bins
  // First bin (around 0) is baseline selection
  TH1* hYieldsPred = new TH1D("hYieldsPred",title+";;N(events)",37,-0.5,36.5);
  hYieldsPred->Sumw2();
  hYieldsPred->GetXaxis()->SetBinLabel(1,"baseline");
  for(int bin = 2; bin <= hYieldsPred->GetNbinsX(); ++bin) {
    TString label = "Bin ";
    label += bin-1;
    hYieldsPred->GetXaxis()->SetBinLabel(bin,label);
  }

  // Muon in control sample
  TH1* hMuonPt = new TH1D("hMuonPt",title+";p_{T}(#mu) [GeV];N(events)",50,0,1200);
  hMuonPt->Sumw2();
  TH1* hMuonEta = new TH1D("hMuonEta",title+";#eta(#mu);N(events)",50,-5,5);
  hMuonEta->Sumw2();



  // --- Analyse the events --------------------------------------------

  // Interface to the event content
  Event* evt = new Event(Sample::fileNameFullSample(id),nEvts);

  // Interface to the muon acceptance maps
  LeptonAcceptance muonAcc(effName,LeptonAcceptance::nameMuonAcc());

  // Interfaces to the muon efficieny maps
  LeptonEfficiency muonRecoEff(effName,LeptonEfficiency::nameMuonRecoEff());
  LeptonEfficiency muonIsoEff(effName,LeptonEfficiency::nameMuonIsoEff());

  // Interfaces to the electron acceptance and efficiencies
  LeptonAcceptance elecAcc(effName,LeptonAcceptance::nameElecAcc());
  LeptonEfficiency elecRecoEff(effName,LeptonEfficiency::nameElecRecoEff());
  LeptonEfficiency elecIsoEff(effName,LeptonEfficiency::nameElecIsoEff());


  // Loop over the events (tree entries)
  while( evt->loadNext() ) {

    // Apply the NJets baseline-cut
    if( !Selection::nJets(evt->nJets()) ) continue;
    // Apply the HT and MHT baseline-cuts
    if( !Selection::ht(evt->ht()) ) continue;
    if( !Selection::mht(evt->mht()) ) continue;
    // Apply the delta-phi cuts
    if( !Selection::deltaPhi(evt->deltaPhi1(),evt->deltaPhi2(),evt->deltaPhi3()) ) continue;

    // Find the search bin (tighter than baseline selection) this event falls into
    const unsigned int searchBin = Selection::searchBin(evt->ht(),evt->mht(),evt->nJets());


    if( evt->isoMuonsN() == 1 && evt->isoElectronsN() == 0 ) { 
      // Select the control sample by requiring exactly one
      // well-reconstructed, isolated muon (and no electrons).
      // On this sample, we can perform the data-based prediction
      // for the number of events with leptons either out-of acceptance,
      // not well reconstructed, or not isolated.

      // Muon acceptance depends on the MHT and N(jets) of the event
      const double muAcc     = muonAcc(evt->mht(),evt->nJets());
      // Muon reconstruction and isolation efficiencies depend on
      // HT, MHT, and N(jets) of the event
      const double muEffIso  = muonIsoEff(evt->ht(),evt->mht(),evt->nJets());
      const double muEffReco = muonRecoEff(evt->ht(),evt->mht(),evt->nJets());

      // Same for the electrons
      const double elAcc     = elecAcc(evt->mht(),evt->nJets());
      const double elEffIso  = elecIsoEff(evt->ht(),evt->mht(),evt->nJets());
      const double elEffReco = elecRecoEff(evt->ht(),evt->mht(),evt->nJets());
      

      // The "lost-lepton weight": product of the probability for the lepton
      // to be out-of acceptance, not reconstructed, or not isolated and
      // a correction to account for the fact that the control sample itself
      // contains only events with well-reconstructed, isolated muons inside
      // the acceptance.
      const double llw = ( lostLeptonProb(muAcc,muEffReco,muEffIso) + lostLeptonProb(elAcc,elEffReco,elEffIso) ) * controlSampleCorr(muAcc,muEffReco,muEffIso);

      // To get a realistic prediction in case of MC, we multiply llw by
      // Event::weight() to normalise to 19.5/fb and the data pile-up
      // profile
      const double fw = llw * evt->weight();

      // Predicted HT, MHT, N(jets) spectra
      hHtPred->Fill(evt->ht(),fw);
      hMhtPred->Fill(evt->mht(),fw);
      hNJetsPred->Fill(evt->nJets(),fw);

      // Predicted event yields      
      hYieldsPred->Fill(0.,fw);	// After the baseline selection
      if( searchBin > 0 ) hYieldsPred->Fill(searchBin,fw); // In the search bin

      // For the control-sample statistics
      hMuonPt->Fill(evt->isoMuonsPt()[0],evt->weight());
      hMuonEta->Fill(evt->isoMuonsEta()[0],evt->weight());

    }

  } // End of loop over events

  // --- Save the Histograms to File -----------------------------------
  TFile outFile("LostLepton_Data_Prediction.root","RECREATE");
  hHtPred->Write();
  hMhtPred->Write();
  hNJetsPred->Write();
  hYieldsPred->Write();
  hMuonPt->Write();
  hMuonEta->Write();
  outFile.Close();
}


double lostLeptonProb(double acc, double reco, double iso) {
  return (1.-acc) + acc*(1.-reco) + acc*reco*(1.-iso);
}

double controlSampleCorr(double acc, double reco, double iso) {
  return 1. / ( acc * reco * iso );
}
