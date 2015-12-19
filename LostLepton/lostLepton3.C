// Perform data-based prediction
// In case of W+jets MC: also fill expectation from MC-truth for closure test
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

#include "../Utils/NTupleReader.h"

// === Declaration of Auxiliary Functions ==============================

// The probability for the muon to be out-of acceptance,
// not reconstructed, or not isolated
double lostLeptonProb(double acc, double reco, double iso);

// Correction factor to account for the fact that the muon
// control sample itself contains only events with well-reconstructed,
// isolated muons inside the acceptance.
double controlSampleCorr(double acc, double reco, double iso);


// === Main Function ===================================================
void lostLepton3(unsigned int id = 11,
		 const TString &effName = "../data/LostLepton_MuonEfficienciesFromWJetMC.root",
		 int nEvts = -1) {

  // --- Declare the Output Histograms ---------------------------------
  const TString title = ( id == 11 ) ? "Lost-#mu Closure-Test" : "Lost-#mu Prediction";

  TH1* hHt = new TH1D("hHt",title+";H_{T} [GeV]",25,500,3000);
  hHt->Sumw2();
  TH1* hHtPred = static_cast<TH1*>(hHt->Clone("hHtPred"));
  TH1* hMht = new TH1D("hMht",title+";#slash{H}_{T} [GeV]",20,200,1200);
  hMht->Sumw2();
  TH1* hMhtPred = static_cast<TH1*>(hMht->Clone("hMhtPred"));
  TH1* hNJets = new TH1D("hNJets",title+";N(jets)",9,3,12);
  hNJets->Sumw2();
  TH1* hNJetsPred = static_cast<TH1*>(hNJets->Clone("hNJetsPred"));

  // Event yields in the different RA2 search bins
  // First bin (around 0) is baseline selection
  TH1* hYields = new TH1D("hYields",title+";;N(events)",37,-0.5,36.5);
  hYields->Sumw2();
  hYields->GetXaxis()->SetBinLabel(1,"baseline");
  for(int bin = 2; bin <= hYields->GetNbinsX(); ++bin) {
    TString label = "Bin ";
    label += bin-1;
    hYields->GetXaxis()->SetBinLabel(bin,label);
  }
  TH1* hYieldsPred = static_cast<TH1*>(hYields->Clone("hYieldsPred"));

  // Muon in control sample
  TH1* hMuonPt = new TH1D("hMuonPt",title+";p_{T}(#mu) [GeV];N(events)",50,0,1200);
  hMuonPt->Sumw2();
  TH1* hMuonEta = new TH1D("hMuonEta",title+";#eta(#mu);N(events)",50,-5,5);
  hMuonEta->Sumw2();



  // --- Analyse the events --------------------------------------------

  std::vector<double> xSecVec; std::vector<int> nEvtVec;
  std::vector<TString> samples = Sample::fileNameFullSample(id, xSecVec, nEvtVec);

  std::cout<<std::endl; timer.Print(); timer.Start();

  // Interface to the muon acceptance maps
  LeptonAcceptance muonAcc(effName,LeptonAcceptance::nameMuonAcc());

  // Interfaces to the muon efficieny maps
  LeptonEfficiency muonRecoEff(effName,LeptonEfficiency::nameMuonRecoEff());
  LeptonEfficiency muonIsoEff(effName,LeptonEfficiency::nameMuonIsoEff());

  // Loop over the events (tree entries)
  for(unsigned int is=0; is<samples.size(); is++){

     TChain * chn = new TChain("stopTreeMaker/AUX");
     chn->Add(samples[is]);

     NTupleReader ntper(chn);

     int toProcessedEvts = nEvts;
     if( nEvts == -1 ) toProcessedEvts = ntper.getNEntries();

     double scaleToLumi = xSecVec[is]*expectedLumi/nEvtVec[is];

     std::cout<<"\nProcessing sample : "<<samples[is]<<std::endl;
     std::cout<<"toProcessedEvts : "<<toProcessedEvts<<"  xSec : "<<xSecVec[is]<<"  oriTotEvt : "<<nEvtVec[is]<<"  expectedLumi : "<<expectedLumi<<"  scaleToLumi : "<<scaleToLumi<<std::endl;

     while(ntper.getNextEvent()){

    // Calculate the jet-based RA2 selection variables
        int selNJet = 0; // Number of jets with pt > 50 GeV and |eta| < 2.5 (`HT jets')
        double selHT = 0; // HT, computed from jets with pt > 50 GeV and |eta| < 2.5 (`HT jets')
        double selMHT = 0; // cmponents of MHT,  computed from jets with pt > 30 GeV (`MHT jets')
        double selMHTx = 0, selMHTy = 0; // x-y cmponents of MHT,  computed from jets with pt > 30 GeV (`MHT jets')

        selNJet = ntper.countJets();
        selHT = ntper.calcHT();
        selMHT = ntper.calcMHT();

        vector<double> compMHTvec = ntper.calcMHTxy();
        selMHTx = compMHTvec[0]; selMHTy = compMHTvec[1];

        double phiMHT = std::atan2(selMHTy, selMHTx);

        vector<double> dphiVec = ntper.calcDPhi( (*ntper.jetsLVec), phiMHT, 3, dphiArr);

        double weight = 1.0;
        weight *= ntper.evtWeight;
        weight *= scaleToLumi;

    // Apply the NJets baseline-cut
        if( !Selection::nJets(selNJet) ) continue;
    // Apply the HT and MHT baseline-cuts
        if( !Selection::ht(selHT) ) continue;
        if( !Selection::mht(selMHT) ) continue;
    // Apply the delta-phi cuts
        if( !Selection::deltaPhi(dphiVec[0],dphiVec[1],dphiVec[2]) ) continue;

    // Find the search bin (tighter than baseline selection) this event falls into
        const unsigned int searchBin = Selection::searchBin(selHT,selMHT,selNJet);

        int selMuons = 0;
        TLorentzVector selMuonLVec;
        for(unsigned int im=0; im<ntper.muonsLVec->size(); ++im){
           if( ntper.muonsLVec->at(im).Pt() > 10 && std::abs(ntper.muonsLVec->at(im).Eta())<2.4 && ntper.muonsRelIso->at(im)
<0.2 ){ ++selMuons; selMuonLVec = ntper.muonsLVec->at(im); }
        }
        int selElectrons = 0;
        for(unsigned int ie=0; ie<ntper.elesLVec->size(); ie++){
           if( ntper.elesLVec->at(ie).Pt() > 10 && std::abs(ntper.elesLVec->at(ie).Eta())<2.4 && ntper.elesRelIso->at(ie)<0.15 ) ++selElectrons;
        }

        if( selMuons == 1 && selElectrons == 0 ){
      // Select the control sample by requiring exactly one
      // well-reconstructed, isolated muon (and no electrons).
      // On this sample, we can perform the data-based prediction
      // for the number of events with muons either out-of acceptance,
      // not well reconstructed, or not isolated.

      // Muon acceptance depends on the MHT and N(jets) of the event
           const double muAcc     = muonAcc(selMHT,selNJet);
      // Muon reconstruction and isolation efficiencies depend on
      // HT, MHT, and N(jets) of the event
           const double muEffIso  = muonIsoEff(selHT,selMHT,selNJet);
           const double muEffReco = muonRecoEff(selHT,selMHT,selNJet);

      // The "lost-lepton weight": product of the probability for the muon
      // to be out-of acceptance, not reconstructed, or not isolated and
      // a correction to account for the fact that the control sample itself
      // contains only events with well-reconstructed, isolated muons inside
      // the acceptance.
           const double llw = lostLeptonProb(muAcc,muEffReco,muEffIso) * controlSampleCorr(muAcc,muEffReco,muEffIso);

      // To get a realistic prediction in case of MC, we multiply llw by
      // Event::weight() to normalise to 19.5/fb and the data pile-up
      // profile
           const double fw = llw * weight;

      // Predicted HT, MHT, N(jets) spectra
           hHtPred->Fill(selHT,fw);
           hMhtPred->Fill(selMHT,fw);
           hNJetsPred->Fill(selNJet,fw);

      // Predicted event yields      
           hYieldsPred->Fill(0.,fw);	// After the baseline selection
           if( searchBin > 0 ) hYieldsPred->Fill(searchBin,fw); // In the search bin

      // For the control-sample statistics
           hMuonPt->Fill(selMuonLVec.Pt(),weight);
           hMuonEta->Fill(selMuonLVec.Eta(),weight);

        } else if( selMuons == 0 ){
      
           if ( id == 11 ) {
	// In this case, we are running over W+jets MC and can perform
	// a closure test: this is an event without well-reconstructed,
	// isolated muons, i.e. it would enter the search region and
	// contribute to the background. Hence, we fill it into the
	// histograms with our expectation for the true background.
	
	// Select only events where the W decayed either
	// - into a muon (pdgId 13)
	// - into a tau (pdgId 15) that decays into a muon
              TLorentzVector genMuLVec;
              bool isWToMuon = false;
              if( !ntper.W_emuVec->empty() ){
                 if( ntper.W_emuVec->size() >1 ) std::cout<<"WARNING ... two W's (info from W_emuVec)?"<<std::endl;
                 for(unsigned int iw=0; iw<ntper.W_emuVec->size(); iw++){
                    if( abs(ntper.genDecayPdgIdVec->at(ntper.W_emuVec->at(iw))) == 13 ){ isWToMuon = true; genMuLVec = ntper.genDecayLVec->at(ntper.W_emuVec->at(iw)); }
                 }
              }
              bool isWToTauToMuon = false;
              if( !ntper.W_tau_emuVec->empty() ){
                 if( ntper.W_tau_emuVec->size() >1 ) std::cout<<"WARNING ... two W's (info from W_tau_emuVec)?"<<std::endl;
                 for(unsigned int iw=0; iw<ntper.W_tau_emuVec->size(); iw++){
                    if( abs(ntper.genDecayPdgIdVec->at(ntper.W_tau_emuVec->at(iw))) == 13 ){ isWToTauToMuon = true; genMuLVec = ntper.genDecayLVec->at(ntper.W_tau_emuVec->at(iw)); }
                 }
              }

	      if( isWToMuon || isWToTauToMuon ) {
	  
	     // True HT, MHT, N(jets) spectra
                 hHt->Fill(selHT,weight);
                 hMht->Fill(selMHT,weight);
                 hNJets->Fill(selNJet,weight);
	  
             // True event yields      
                 hYields->Fill(0.,weight);	// After the baseline selection
                 if( searchBin > 0 ) hYields->Fill(searchBin,weight); // In the search bin
	  
	      }
           }
        }
     } // End of loop over events

     if( chn ) delete chn;
  }

  // --- Save the Histograms to File -----------------------------------
  const TString outName = ( id == 11 ) ? "LostLepton_ClosureMuon.root" : "LostLepton_DataPrediction.root";
  TFile outFile(outName,"RECREATE");
  hHt->Write();
  hMht->Write();
  hNJets->Write();
  hYields->Write();
  hHtPred->Write();
  hMhtPred->Write();
  hNJetsPred->Write();
  hYieldsPred->Write();
  hMuonPt->Write();
  hMuonEta->Write();
  outFile.Close();
}

double lostLeptonProb(double acc, double reco, double iso) {
  return 1.; // to be fixed
}

double controlSampleCorr(double acc, double reco, double iso) {
  return 1.; // to be fixed
}
