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

//#include "../Utils/Event.h"
#include "../Utils/Sample.h"
#include "../Utils/NTupleReader.h"

// Investigation of SM background and signal event properties.
// Learning how to access the data
//
//
// Author: Matthias Schroeder
//         matthias.schroeder@AT@desy.de
//         November 2013

// Revision: Kenichi Hatakeyama
//           Kenichi_Hatakeyama@baylor.edu
//           January 2016

// === Main Function ===================================================
void general1(unsigned int id, int nEvts = -1) {
  std::cout << "Analysing the '" << Sample::toTString(id) << "' sample" << std::endl;


  // --- Declare the Output Histograms ---------------------------------
  TH1* hNJets = new TH1D("hNJets",";N(jets);N(events)",12,0,12);
  hNJets->Sumw2();
  TH1* hHt = new TH1D("hHt",";H_{T} [GeV]",30,0,3000);
  hHt->Sumw2();
  hHt->GetXaxis()->SetNdivisions(505);
  TH1* hMht = new TH1D("hMht",";#slash{H}_{T} [GeV]",40,0,1200);
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

        int selMuons = 0;	
        for(unsigned int im=0; im<ntper.muonsLVec->size(); ++im){
	  if( ntper.muonsLVec->at(im).Pt() > 10 && std::abs(ntper.muonsLVec->at(im).Eta())<2.4 && ntper.muonsMiniIso->at(im)<0.2) ++selMuons;
        }

        int selElectrons = 0;
        for(unsigned int ie=0; ie<ntper.elesLVec->size(); ie++){
	  if( ntper.elesLVec->at(ie).Pt() > 10 && std::abs(ntper.elesLVec->at(ie).Eta())<2.4 && ntper.elesMiniIso->at(ie)<0.1) ++selElectrons;
        }
        if( selMuons != 0 ) continue;
        if( selElectrons !=0 ) continue;

	// Obtain the jet-based RA2 selection variables
        int selNJet = ntper.NJets; // Number of jets with pt > 30 GeV and |eta| < 2.4 (`HT jets')
        double selHT = ntper.HT;   // HT, computed from jets with pt > 30 GeV and |eta| < 2.4 (`HT jets')
        double selMHT = ntper.MHT; // MHT,  computed from jets with pt > 30 GeV (`MHT jets')

        double weight = 1.0;
	if (id!=1){  // no scaling for data
	  weight *= ntper.evtWeight;
	  //weight *= scaleToLumi;
	}
	
	// Calculate the MHTx, MHTy
        double selMHTx = 0, selMHTy = 0; // x-y cmponents of MHT,  computed from jets with pt > 30 GeV (`MHT jets')
        vector<double> compMHTvec = ntper.calcMHTxy();
        selMHTx = compMHTvec[0]; selMHTy = compMHTvec[1];        
        double phiMHT = std::atan2(selMHTy, selMHTx);

	//>>> PLACE DELTA PHI COMPUTATION HERE


	// Actually, the deltaPhi(MHT,jet{1,2,3,4}) variables are already stored in ntuples, 
	// so we can cross-check your deltaPhi computations
	/*
        vector<double> dphiVec; 
	dphiVec.insert(dphiVec.end(), 
		       {ntper.DeltaPhi1,ntper.DeltaPhi2,ntper.DeltaPhi3,ntper.DeltaPhi4});
	std::cout << dphiVec.size() << " " << deltaPhis.size() << std::endl;
	
	if (std::equal ( dphiVec.begin(), dphiVec.end(), deltaPhis.begin() )){
	  printf("jetpt:                  %8.4f, %8.4f, %8.4f, %8.4f\n",
		 ntper.jetsLVec->at(0).Pt(), ntper.jetsLVec->at(1).Pt(), ntper.jetsLVec->at(2).Pt(), ntper.jetsLVec->at(3).Pt());
	  printf("jeteta:                 %8.4f, %8.4f, %8.4f, %8.4f\n",
		 ntper.jetsLVec->at(0).Eta(),ntper.jetsLVec->at(1).Eta(),ntper.jetsLVec->at(2).Eta(),ntper.jetsLVec->at(3).Eta());
	  printf("deltaPhis (computed):   %12.8e, %18.14e, %18.14e, %18.14e\n",deltaPhis[0],deltaPhis[1],deltaPhis[2],deltaPhis[3]);
	  printf("dphiVec (from ntuples): %12.8e, %18.14e, %18.14e, %18.14e\n",
		 dphiVec[0],dphiVec[1],dphiVec[2],dphiVec[3]);
	}
	*/

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
	if (deltaPhis.size()>0) hDeltaPhi.at(0)->Fill(deltaPhis.at(0),weight);
	if (deltaPhis.size()>1) hDeltaPhi.at(1)->Fill(deltaPhis.at(1),weight);
	if (deltaPhis.size()>2) hDeltaPhi.at(2)->Fill(deltaPhis.at(2),weight);
	if (deltaPhis.size()>3) hDeltaPhi.at(3)->Fill(deltaPhis.at(3),weight);
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
