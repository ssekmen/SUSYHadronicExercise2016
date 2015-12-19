// Perform closure test for muon acceptance in W+Jets MC sample
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
#include "../Utils/Sample.h"
#include "../Utils/Selection.h"

#include "../Utils/NTupleReader.h"

// === Main Function ===================================================
void lostLepton2(unsigned int id = 11,
		 const TString &effName = "../data/LostLepton_MuonEfficienciesFromWJetMC.root",
		 int nEvts = -1) {

  // --- Declare the Output Histograms ---------------------------------
  const TString title = "Acceptance Closure-Test";

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

    // Select only events where the W decayed
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
        if( !( isWToMuon || isWToTauToMuon ) ) continue;

        const double genMuPt = genMuLVec.Pt();
        const double genMuEta = genMuLVec.Eta();
        const double genMuPhi = genMuLVec.Phi();

    // Find the search bin (tighter than baseline selection) this event falls into
        const unsigned int searchBin = Selection::searchBin(selHT, selMHT, selNJet);

        if( genMuPt > LeptonAcceptance::muonPtMin() && std::abs(genMuEta) < LeptonAcceptance::muonEtaMax() ) {

      // Generator-level muon in acceptance: our control sample.
      // The event will be weighted to account for the probability that the muon
      // is out-of acceptance. In this way, we can predict the number
      // of events with muons out-of-acceptance

      // The muon acceptance depends on the MHT and N(jets) of the event
           const double muAcc = muonAcc(selMHT, selNJet);

      // The "lost-lepton weight": product of the probability for the muon
      // to be out-of acceptance (1.-muAcc) and a correction to account for the fact
      // that the control sample itself contains only events with muons
      // inside the acceptance (1./muAcc).
           const double llw = (1. - muAcc) / muAcc;

      // Predicted HT, MHT, N(jets) spectra
           hHtPred->Fill(selHT,llw*weight);
           hMhtPred->Fill(selMHT,llw*weight);
           hNJetsPred->Fill(selNJet,llw*weight);

      // Predicted event yields
           hYieldsPred->Fill(0.,llw*weight);        // After the baseline selection
           if( searchBin > 0 ) hYieldsPred->Fill(searchBin,llw*weight); // In the search bin

      // For the control-sample statistics
           hMuonPt->Fill(genMuPt, weight);
           hMuonEta->Fill(genMuEta, weight);

        } else {
      // The muon is out-of acceptance. This is the truth to which we
      // want to compare our prediction.

      // True HT, MHT, N(jets) spectra
           hHt->Fill(selHT, weight);
           hMht->Fill(selMHT, weight);
           hNJets->Fill(selNJet, weight);

      // True event yields
           hYields->Fill(0., weight);        // After the baseline selection
           if( searchBin > 0 ) hYields->Fill(searchBin, weight); // In the search bin
        }
     }
     if( chn ) delete chn;
  } // End of loop over events

  // --- Save the Histograms to File -----------------------------------
  TFile outFile("LostLepton_ClosureMuonAcceptance.root","RECREATE");
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
