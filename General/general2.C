#include <cmath>
#include <iostream>
#include <vector>

#include "TChain.h"
#include "TFile.h"
#include "TH1.h"
#include "TH1D.h"
#include "TString.h"
#include "TVector2.h"
#include "TLorentzVector.h"
#include "TFileCollection.h"

//KH #include "../Utils/Event.h"
#include "../Utils/Sample.h"
#include "../Utils/Selection.h"
#include "../Utils/NTupleReader.h"

// Simulated background and data yields.
//
// Author: Matthias Schroeder
//         matthias.schroeder@AT@desy.de
//         November 2013

// Revision: Kenichi Hatakeyama
//           Kenichi_Hatakeyama@baylor.edu
//           January 2016

// === Main Function ===================================================
void general2(unsigned int id, int nEvts = -1) {
  std::cout << "Analysing the '" << Sample::toTString(id) << "' sample" << std::endl;


  // --- Declare the Output Histograms ---------------------------------
  TH1* hNJets = new TH1D("hNJets",";N(jets);N(events)",9,3,12);
  hNJets->Sumw2();
  TH1* hHt = new TH1D("hHt",";H_{T} [GeV]",50,500,3000);
  hHt->Sumw2();
  hHt->GetXaxis()->SetNdivisions(505);
  TH1* hMht = new TH1D("hMht",";#slash{H}_{T} [GeV]",40,200,1400);
  hMht->Sumw2();
  hMht->GetXaxis()->SetNdivisions(505);
  std::vector<TH1*> hJetPt(4,NULL);
  std::vector<TH1*> hJetPhi(4,NULL);
  std::vector<TH1*> hJetEta(4,NULL);
  std::vector<TH1*> hDeltaPhi(4,NULL);
  for(unsigned int i = 0; i < hJetPt.size(); ++i) {
    TString name = "hJetPt_";
    name += i;
    TString title = ";p_{T}(jet ";
    title += i+1;
    title += ") [GeV];N(events)";
    hJetPt.at(i) = new TH1D(name,title,50,0,1500);
    hJetPt.at(i)->Sumw2();

    name = "hJetPhi_";
    name += i;
    title = ";#phi(jet ";
    title += i+1;
    title += ");N(events)";
    hJetPhi.at(i) = new TH1D(name,title,24,-4,4);
    hJetPhi.at(i)->Sumw2();

    name = "hJetEta_";
    name += i;
    title = ";#eta(jet ";
    title += i+1;
    title += ");N(events)";
    hJetEta.at(i) = new TH1D(name,title,25,-5,5);
    hJetEta.at(i)->Sumw2();

    name = "hDeltaPhi_";
    name += i;
    title = ";#Delta#phi(#slash{#vec{H}}_{T},jet ";
    title += i+1;
    title += ");N(events)";
    hDeltaPhi.at(i) = new TH1D(name,title,40,0,4);
    hDeltaPhi.at(i)->Sumw2();
  }

  // Event yields in the different RA2 search bins
  // First bin (around 0) is baseline selection
  TH1* hYields = new TH1D("hYields",";;N(events)",7,-0.5,6.5);
  hYields->Sumw2();
  hYields->GetXaxis()->SetBinLabel(1,"baseline");
  for(int bin = 2; bin <= hYields->GetNbinsX(); ++bin) {
    TString label = "Bin ";
    label += bin-1;
    hYields->GetXaxis()->SetBinLabel(bin,label);
  }

  std::vector<double> xSecVec; std::vector<int> nEvtVec;
  std::vector<TString> samples = Sample::fileNameFullSample(id, xSecVec, nEvtVec);

  for(unsigned int is=0; is<samples.size(); is++){

    TChain * chn = new TChain("tree");
     chn->Add(samples[is]);

     NTupleReader ntper(chn);

     int toProcessedEvts = nEvts;
     if( nEvts == -1 ) toProcessedEvts = ntper.getNEntries();

     double scaleToLumi = xSecVec[is]*expectedLumi/nEvtVec[is];

     std::cout<<"\nProcessing sample : "<<samples[is]<<std::endl;
     std::cout<<"toProcessedEvts : "<<toProcessedEvts<<"  xSec : "<<xSecVec[is]<<"  oriTotEvt : "<<nEvtVec[is]<<"  expectedLumi : "<<expectedLumi<<"  scaleToLumi : "<<scaleToLumi<<std::endl;

     while(ntper.getNextEvent()){

        int nevtProcessed = ntper.getNevtProcessed();

        if( ntper.getNevtProcessed() > toProcessedEvts ) break;

        if( nevtProcessed == 1 || nevtProcessed == toProcessedEvts || nevtProcessed%(toProcessedEvts/10) ==0 ) std::cout<<"  processing the "<<nevtProcessed<<"th event"<<std::endl;

    // Apply the event cleaning
	if (id<31 || id>37){ // if not fastsim
	  if (!(ntper.vtxSize>0)) continue;
	  if (!(ntper.eeBadScFilter)) continue;
	  if (!(ntper.HBHENoiseFilter)) continue;
	  if (!(ntper.HBHEIsoNoiseFilter)) continue;
	  if (!(ntper.JetID)) continue;
	} 
	if (id==1) {         // if data
	  if (!(ntper.CSCTightHaloFilter)) continue;
	}

    // Apply lepton vetoes
        int selMuons = 0;
        for(unsigned int im=0; im<ntper.muonsLVec->size(); im++){
           if( ntper.muonsLVec->at(im).Pt() > 10 && std::abs(ntper.muonsLVec->at(im).Eta())<2.4 && ntper.muonsMiniIso->at(im)<0.2 ) selMuons++;
        }
        int selElectrons = 0;
        for(unsigned int ie=0; ie<ntper.elesLVec->size(); ie++){
           if( ntper.elesLVec->at(ie).Pt() > 10 && std::abs(ntper.elesLVec->at(ie).Eta())<2.4 && ntper.elesMiniIso->at(ie)<0.1 ) selElectrons++;
        }
        if( selMuons != 0 ) continue;
        if( selElectrons !=0 ) continue;

	// Obtain the jet-based RA2 selection variables
        int selNJet = ntper.NJets; // Number of jets with pt > 30 GeV and |eta| < 2.4 (`HT jets')
        double selHT = ntper.HT;   // HT, computed from jets with pt > 30 GeV and |eta| < 2.4 (`HT jets')
        double selMHT = ntper.MHT; // cmponents of MHT,  computed from jets with pt > 30 GeV (`MHT jets')
        vector<double> dphiVec; 
	dphiVec.insert(dphiVec.end(), 
		       {ntper.DeltaPhi1,ntper.DeltaPhi2,ntper.DeltaPhi3,ntper.DeltaPhi4});

        double weight = 1.0;
	if (id!=1){  // no scaling for data
	  weight *= scaleToLumi;
	  /*
	  if (nevtProcessed==1 && (id==11 || id==12 || id==13 || id==14 || id==21 || id==22 )){
	    printf("scaleToLumi (computed): %10.5e, (from ntuple): %10.5e\n",scaleToLumi,ntper.evtWeight*expectedLumi);
	  }
	  */
	}

    // Apply the NJets baseline-cut
        if( !Selection::nJets(selNJet) ) continue;

    // Apply the HT and MHT baseline-cuts
        if( !Selection::ht(selHT) ) continue;
        if( !Selection::mht(selMHT) ) continue;

    // Apply the delta-phi cuts
        if( !Selection::deltaPhi(dphiVec[0],dphiVec[1],dphiVec[2],dphiVec[3]) ) continue;

    // Apply the isotrack vetoes
	if (!(ntper.isoMuonTracks==0)) continue;
	if (!(ntper.isoElectronTracks==0)) continue;
	if (!(ntper.isoPionTracks==0)) continue;

    // Fill histograms
        hNJets->Fill(selNJet, weight);
        hHt->Fill(selHT, weight);
        hMht->Fill(selMHT, weight);
        for(unsigned int ih = 0; ih < hJetPt.size(); ++ih) {
           if( ih == ntper.jetsLVec->size() ) break;
           hJetPt.at(ih)->Fill(ntper.jetsLVec->at(ih).Pt(), weight);
           hJetEta.at(ih)->Fill(ntper.jetsLVec->at(ih).Eta(), weight);
           hJetPhi.at(ih)->Fill(ntper.jetsLVec->at(ih).Phi(), weight);
        }

        for(unsigned int ih=0; ih < hDeltaPhi.size(); ++ih) hDeltaPhi[ih]->Fill(dphiVec[ih], weight);

        hYields->Fill(0.,weight);	// This is after the baseline selection

    // Apply the search-bin selection (tighter than baseline)
       const unsigned int searchBin = Selection::searchBin(selHT,selMHT,selNJet,ntper.BTags);
       if( searchBin > 0 ) {
	 hYields->Fill(searchBin,weight);
       }
     }
     if( chn ) delete chn;
  } // End of loop over events

  // --- Save the Histograms to File -----------------------------------
  TFile outFile("General_"+Sample::toTString(id)+"-Yields.root","RECREATE");
  hNJets->Write();
  hHt->Write();
  hMht->Write();
  for(unsigned int i = 0; i < hJetPt.size(); ++i) {
    hJetPt.at(i)->Write();
    hJetEta.at(i)->Write();
    hJetPhi.at(i)->Write();
    hDeltaPhi.at(i)->Write();
  }  
  hYields->Write();
  outFile.Close();
}
