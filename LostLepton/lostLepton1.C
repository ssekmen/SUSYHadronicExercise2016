// Determine the acceptance and the identification and isolation efficiencies
// of the muon in simulated W+jet events
//
// For this exercise, we will need in particular the following
// event variables:
// flgW(), flgTau()             : characterise the W and possible subsequent tau decay
// genLeptonPt,Eta,Phi()        : kinematic properties of generator-level lepton from 
//                                W decay
// genLeptonFromTauPt,Eta,Phi() : kinematic properties of generator-level tau from lepton
//                                from W decay
// muonsN,Pt,Eta,Phi()          : (array) kinematic properties of well-reconstructed muons
// isoMuonsN,Pt,Eta,Phi()       : (array) kinematic properties of well-reconstructed and
//                                isolated muons
//
// Author: Matthias Schroeder
//         matthias.schroeder@AT@desy.de
//         November 2013

#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

#include "TFile.h"
#include "TH1.h"
#include "TH1D.h"
#include "TH2.h"
#include "TH2D.h"
#include "TString.h"
#include "TVector2.h"

#include "../Utils/Event.h"
#include "../Utils/LeptonAcceptance.h"
#include "../Utils/LeptonEfficiency.h"
#include "../Utils/Sample.h"
#include "../Utils/Selection.h"
#include "../Utils/utils.h"

#include "../Utils/NTupleReader.h"

// === Global Variables ================================================

// Muon-matching related parameters
const double deltaRMax = 0.1;
const double deltaPtMax = 0.2;


// === Main Function ===================================================
void lostLepton1(unsigned int id = 11, int nEvts = -1) {

  // --- Declare the Output Histograms ---------------------------------
  // We want to determine efficiencies, so we always have
  // two histograms: the distribution before and after the
  // respective selection cut

  // For muon-acceptance determination. The acceptance is determined
  // in bins of MHT and N(jets)
  std::vector<double> accMhtBins = LeptonAcceptance::mhtBinEdges();
  std::vector<double> accNJetsBins = LeptonAcceptance::nJetsBinEdges();
  TH2* hNJetsVsMhtAll = new TH2D("hNJetsVsMhtAll",";#slash{H}_{T} [GeV];N(jets)",
				 accMhtBins.size()-1,&(accMhtBins.front()),
				 accNJetsBins.size()-1,&(accNJetsBins.front()));
  hNJetsVsMhtAll->Sumw2();
  TH2* hNJetsVsMhtInAcc = static_cast<TH2*>(hNJetsVsMhtAll->Clone("hNJetsVsMhtInAcc"));


  // For muon reconstruction and isolation efficiencies. The efficiencies are
  // determined in bins of HT, MHT, and N(jets)
  std::vector<TH2*> hRecoAll;
  std::vector<TH2*> hRecoPass;
  std::vector<TH2*> hIsoAll;
  std::vector<TH2*> hIsoPass;
  for(unsigned int i = 0; i < LeptonEfficiency::numNJetBins(); ++i) {
    std::vector<double> effHTBins = LeptonEfficiency::htBinEdges(i); 
    std::vector<double> effMHTBins = LeptonEfficiency::mhtBinEdges(i); 
    
    hRecoAll.push_back( new TH2D( "hReco"+LeptonEfficiency::nJetBinId(i)+"All",";H_{T} [GeV];#slash{H}_{T} [GeV]",
				  effHTBins.size()-1,&(effHTBins.front()),
				  effMHTBins.size()-1,&(effMHTBins.front()) ) );
    hRecoAll.back()->Sumw2(); 
    hRecoAll.back()->GetXaxis()->SetNdivisions(505);
    hRecoPass.push_back( static_cast<TH2*>(hRecoAll.back()->Clone("hReco"+LeptonEfficiency::nJetBinId(i)+"Pass")) );

    hIsoAll.push_back( new TH2D( "hIso"+LeptonEfficiency::nJetBinId(i)+"All",";H_{T} [GeV];#slash{H}_{T} [GeV]",
				 effHTBins.size()-1,&(effHTBins.front()),
				 effMHTBins.size()-1,&(effMHTBins.front()) ) );
    hIsoAll.back()->Sumw2(); 
    hIsoAll.back()->GetXaxis()->SetNdivisions(505);
    hIsoPass.push_back( static_cast<TH2*>(hIsoAll.back()->Clone("hIso"+LeptonEfficiency::nJetBinId(i)+"Pass")) );
  }



  // --- Analyse the events --------------------------------------------

  std::vector<double> xSecVec; std::vector<int> nEvtVec;
  std::vector<TString> samples = Sample::fileNameFullSample(id, xSecVec, nEvtVec);

  std::cout<<std::endl; timer.Print(); timer.Start();

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

    // Acceptance determination 1: Counter for all events
    // with muons at generator level
        hNJetsVsMhtAll->Fill(selMHT, selNJet, weight);

    // Check if generator-level muon is in acceptance
        if( genMuPt > LeptonAcceptance::muonPtMin() && std::abs(genMuEta) < LeptonAcceptance::muonEtaMax() ) {

      // Acceptance determination 2: Counter for only those events
      // with generator-level muons inside acceptance
           hNJetsVsMhtInAcc->Fill(selMHT, selNJet, weight);

      // Reconstruction-efficiency determination 1: Counter for all events 
      // with generator-level muons inside acceptance, regardless of whether
      // the muon has also been reconstructed or not.
           const unsigned int nJetIdx = LeptonEfficiency::nJetBin(selNJet);
           hRecoAll.at(nJetIdx)->Fill(selHT, selMHT, weight);
      
      // Check if the muon has been reconstructed: check if a reconstructed 
      // muon is present in the event that matches the generator-level muon
      // in R and in pt
           int matchedMuonIdx = -1;
           if( utils::findMatchedObject(matchedMuonIdx,genMuEta,genMuPhi,(*ntper.muonsLVec),deltaRMax) ) {
	// Muon is reconstructed
              const double relDeltaPtMu = std::abs(genMuPt - ntper.muonsLVec->at(matchedMuonIdx).Pt()) / ntper.muonsLVec->at(matchedMuonIdx).Pt();
              if( relDeltaPtMu < deltaPtMax ) {
	  // and matches generated pt

	  // Reconstruction-efficiency determination 2: Counter for those events 
	  // with generator-level muons inside acceptance where the muon has also
	  // been reconstructed.
                 hRecoPass.at(nJetIdx)->Fill(selHT, selMHT, weight);

                 hIsoAll.at(nJetIdx)->Fill(selHT, selMHT, weight);
                 if( ntper.muonsRelIso->at(matchedMuonIdx) < 0.2 ){
                    hIsoPass.at(nJetIdx)->Fill(selHT, selMHT, weight);
                 }
              } // End of pt matching
           }	// End of reconstructed muon
        } // End of muon in acceptance
     } // End of loop over events
     if( chn ) delete chn;
  }
  std::cout << "ok" << std::endl;

  // Compute acceptance
  TH2* hAcc = static_cast<TH2*>(hNJetsVsMhtInAcc->Clone(LeptonAcceptance::nameMuonAcc()));
  hAcc->Divide(hNJetsVsMhtAll);

  // Compute efficiencies
  std::vector<TH2*> hRecoEff;
  std::vector<TH2*> hIsoEff;
  for(unsigned int i = 0; i < LeptonEfficiency::numNJetBins(); ++i) {
    TH2* h = static_cast<TH2*>(hRecoPass.at(i)->Clone(LeptonEfficiency::nameMuonRecoEff(i)));
    h->Divide(hRecoAll.at(i));
    hRecoEff.push_back(h);
    
    h = static_cast<TH2*>(hIsoPass.at(i)->Clone(LeptonEfficiency::nameMuonIsoEff(i)));
    h->Divide(hIsoAll.at(i));
    hIsoEff.push_back(h);
  }  
  
  // --- Save the Histograms to File -----------------------------------
  TFile outFile("../data/LostLepton_MuonEfficienciesFromWJetMC.root","RECREATE");
  hAcc->Write();
  for(unsigned int i = 0; i < hRecoEff.size(); ++i) {
    hRecoEff.at(i)->Write();
    hIsoEff.at(i)->Write();
  }
  for(unsigned int i = 0; i < hRecoAll.size(); ++i) {
    hRecoAll.at(i)->Write();
    hRecoPass.at(i)->Write();
    hIsoAll.at(i)->Write();
    hIsoPass.at(i)->Write();
  }
  outFile.Close();
}
