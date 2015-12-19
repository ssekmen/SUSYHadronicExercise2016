#include <cmath>
#include <iostream>
#include <vector>

#include "TH1.h"
#include "TH1D.h"
#include "TString.h"
#include "TVector2.h"

#include "../Utils/Event.h"
#include "../Utils/Sample.h"
#include "../Utils/NTupleReader.h"


// Investigation of SM background and signal event properties.
// Learning how to access the data
//
//
// Author: Matthias Schroeder
//         matthias.schroeder@AT@desy.de
//         November 2013

// === Main Function ===================================================
void general1(unsigned int id, int nEvts = -1) {
  std::cout << "Analysing the '" << Sample::toTString(id) << "' sample" << std::endl;


  // --- Declare the Output Histograms ---------------------------------
  TH1* hNJets = new TH1D("hNJets",";N(jets);N(events)",12,0,12);
  hNJets->Sumw2();
  TH1* hHt = new TH1D("hHt",";H_{T} [GeV]",30,0,3000);
  hHt->Sumw2();
  hHt->GetXaxis()->SetNdivisions(505);
  TH1* hMht = new TH1D("hMht",";#slash{H}_{T} [GeV]",24,0,1200);
  hMht->Sumw2();
  hMht->GetXaxis()->SetNdivisions(505);
  std::vector<TH1*> hJetPt(3,NULL);
  std::vector<TH1*> hJetPhi(3,NULL);
  std::vector<TH1*> hJetEta(3,NULL);
  std::vector<TH1*> hDeltaPhi(3,NULL);
  for(unsigned int i = 0; i < hJetPt.size(); ++i) {
    TString name = "hJetPt_";
    name += i;
    TString title = ";p_{T}(jet ";
    title += i+1;
    title += ") [GeV];N(events)";
    hJetPt.at(i) = new TH1D(name,title,30,0,1500);
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
    hDeltaPhi.at(i) = new TH1D(name,title,24,0,4);
    hDeltaPhi.at(i)->Sumw2();
  }

  // --- Analyse the events --------------------------------------------
  // The Event class is an interface to the TTree objects in the
  // input ROOT files. 
  // Here, we use the following event variables:
  //  - isoElectronsN() : Number of well-reconstructed, isolated electrons
  //  - isoMuonsN()     : Number of well-reconstructed, isolated muons
  //  - jetsN()         : Number of well-reconstructed jets
  //  - jetsPt()        : Array of size jetsN() lsstoring the jets pt; likewise for eta and phi

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

        int nevtProcessed = ntper.getNevtProcessed();

        if( ntper.getNevtProcessed() > toProcessedEvts ) break;

        if( nevtProcessed == 1 || nevtProcessed == toProcessedEvts || nevtProcessed%(toProcessedEvts/10) ==0 ) std::cout<<"  processing the "<<nevtProcessed<<"th event"<<std::endl;

        int selMuons = 0;
        for(unsigned int im=0; im<ntper.muonsLVec->size(); ++im){
           if( ntper.muonsLVec->at(im).Pt() > 10 && std::abs(ntper.muonsLVec->at(im).Eta())<2.4 && ntper.muonsRelIso->at(im)<0.2 ) ++selMuons;
        }
        int selElectrons = 0;
        for(unsigned int ie=0; ie<ntper.elesLVec->size(); ie++){
//          bool isEB = std::abs(elesLVec->at(ie).Eta()) < 1.479 ? true : false;
//           unsigned int idx = isEB ? 0 : 1;
           if( ntper.elesLVec->at(ie).Pt() > 10 && std::abs(ntper.elesLVec->at(ie).Eta())<2.4 && ntper.elesRelIso->at(ie)<0.15 ) ++selElectrons;
        }
        if( selMuons != 0 ) continue;
        if( selElectrons !=0 ) continue;

	// Calculate the jet-based RA2 selection variables
        int selNJet = 0; // Number of jets with pt > 50 GeV and |eta| < 2.5 (`HT jets')
        double selHT = 0; // HT, computed from jets with pt > 50 GeV and |eta| < 2.5 (`HT jets')
        double selMHT = 0; // cmponents of MHT,  computed from jets with pt > 30 GeV (`MHT jets')
        double selMHTx = 0, selMHTy = 0; // x-y cmponents of MHT,  computed from jets with pt > 30 GeV (`MHT jets')

        selNJet = ntper.countJets();
        selHT = ntper.calcHT();

        vector<double> compMHTvec = ntper.calcMHTxy();
        selMHTx = compMHTvec[0]; selMHTy = compMHTvec[1];
        
        selMHT = sqrt(selMHTx*selMHTx + selMHTy*selMHTy);
        double phiMHT = std::atan2(selMHTy, selMHTx);

        double weight = 1.0;
        weight *= ntper.evtWeight;
        weight *= scaleToLumi;

	// Skipping one problematic QCD event in the low HT sample (MHT ~ 715.595 GeV)
        if( id == 14 && is ==0 && ntper.run == 1 && ntper.lumi == 119397 && ntper.event == 11933645 ) continue;
	//>>> PLACE DELTA PHI COMPUTATION HERE

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

     }

     if(chn) delete chn;

     std::cout<<std::endl; timer.Stop(); timer.Print(); timer.Continue();
  }

  // --- Save the Histograms to File -----------------------------------
  TFile outFile("General_"+Sample::toTString(id)+".root","RECREATE");
  hNJets->Write();
  hHt->Write();
  hMht->Write();
  for(unsigned int i = 0; i < hJetPt.size(); ++i) {
    hJetPt.at(i)->Write();
    hJetEta.at(i)->Write();
    hJetPhi.at(i)->Write();
    hDeltaPhi.at(i)->Write();
  }  
  outFile.Close();
}
