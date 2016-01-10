#define ExpectationMaker_cxx
#include "ExpectationMaker.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void ExpectationMaker::Run(std::string outputFileName, double HTgen_cut)
{

  // setup output tree
  tExpectation_ = new TTree("mc_exp_tree","tree for calculating lepton efficiencies and expected number of background events from MC");

  tExpectation_->Branch("EvtNum",&EvtNum,"EvtNum/i");
  tExpectation_->Branch("HT",&HT);
  tExpectation_->Branch("MHT",&MHT);
  tExpectation_->Branch("NJets",&NJets);
  tExpectation_->Branch("BTags",&BTags);
  //  tExpectation_->Branch("Leptons",&Leptons);
  tExpectation_->Branch("isoTracks",&isoTracks);
  //  tExpectation_->Branch("isoTrack_highestPt",&isoTracks);
  tExpectation_->Branch("Bin",&Bin_);
  tExpectation_->Branch("genHT",&genHT);
  tExpectation_->Branch("NVtx",&NVtx);
  tExpectation_->Branch("DeltaPhi1",&DeltaPhi1);
  tExpectation_->Branch("DeltaPhi2",&DeltaPhi2);
  tExpectation_->Branch("DeltaPhi3",&DeltaPhi3);
  tExpectation_->Branch("DeltaPhi4",&DeltaPhi4);
  tExpectation_->Branch("Weight", &Weight);
  tExpectation_->Branch("MET",&METPt);
  tExpectation_->Branch("METPhi",&METPhi);
  tExpectation_->Branch("GenMuNum",&GenMuNum_);
  tExpectation_->Branch("GenMus", "std::vector<TLorentzVector>", &GenMus,32000,0);
  tExpectation_->Branch("GenMu_MT2Activity", &GenMu_MT2Activity);
  tExpectation_->Branch("GenElecNum",&GenElecNum_);
  tExpectation_->Branch("GenEls", "std::vector<TLorentzVector>", &GenEls, 32000, 0);
  tExpectation_->Branch("GenElec_MT2Activity", &GenElec_MT2Activity);
  tExpectation_->Branch("Expectation",&Expectation);  
  tExpectation_->Branch("ExpectationReductionIsoTrack",&ExpectationReductionIsoTrack);
  tExpectation_->Branch("muAcc",&muAcc);  
  tExpectation_->Branch("muReco",&muReco);  
  tExpectation_->Branch("muIso",&muIso);  
  tExpectation_->Branch("MTW",&mtw);  
  //  tExpectation_->Branch("IsoTrackMTW",&isoTrackMTW_); 
  tExpectation_->Branch("elecAcc",&elecAcc);  
  tExpectation_->Branch("elecReco",&elecReco);  
  tExpectation_->Branch("elecIso",&elecIso);  
  //  tExpectation_->Branch("muIsoTrack",&muIsoTrack);  
  //  tExpectation_->Branch("MuPurity",&MuPurity_); 
  //  tExpectation_->Branch("ElecPurity",&ElecPurity_); 
  tExpectation_->Branch("selectedIDIsoMuonsNum",&selectedIDIsoMuonsNum_);
  tExpectation_->Branch("selectedIDIsoMuons", "std::vector<TLorentzVector>", &Muons, 32000, 0);
  tExpectation_->Branch("muIsoMatched", &muIsoMatched);
  tExpectation_->Branch("selectedIDIsoMuons_MTW", &selectedIDIsoMuons_MTW);
  tExpectation_->Branch("selectedIDIsoMuons_MT2Activity", &selectedIDIsoMuons_MT2Activity);
  tExpectation_->Branch("selectedIDMuonsNum",&selectedIDMuonsNum_);
  tExpectation_->Branch("selectedIDMuons", "std::vector<TLorentzVector>", &selectedIDMuons, 32000, 0);
  tExpectation_->Branch("selectedIDMuonsMatched", &muRecoMatched);
  tExpectation_->Branch("selectedIDMuons_MTW", &selectedIDMuons_MTW);
  tExpectation_->Branch("selectedIDMuons_MiniIso", &selectedIDMuons_MiniIso);
  //  tExpectation_->Branch("selectedIDMuons_MT2Activity", &selectedIDMuons_MT2Activity);
  tExpectation_->Branch("selectedIDIsoElectronsNum",&selectedIDIsoElectronsNum_);
  tExpectation_->Branch("selectedIDIsoElectrons", "std::vector<TLorentzVector>", &Electrons, 32000, 0);
  tExpectation_->Branch("elecIsoMatched", &elecIsoMatched);
  tExpectation_->Branch("selectedIDIsoElectrons_MTW", &selectedIDIsoElectrons_MTW);
  tExpectation_->Branch("selectedIDIsoElectrons_MT2Activity", &selectedIDIsoElectrons_MT2Activity);
  tExpectation_->Branch("selectedIDElectronsNum",&selectedIDElectronsNum_);
  tExpectation_->Branch("selectedIDElectrons", "std::vector<TLorentzVector>", &selectedIDElectrons, 32000, 0);
  tExpectation_->Branch("selectedIDElectronsMatched", &elecRecoMatched);
  tExpectation_->Branch("selectedIDElectrons_MTW", &selectedIDElectrons_MTW);
  tExpectation_->Branch("selectedIDElectrons_MiniIso", &selectedIDElectrons_MiniIso);
  //  tExpectation_->Branch("selectedIDElectrons_MT2Activity", &selectedIDElectrons_MT2Activity);
 
  tExpectation_->Branch("isoElectronTracks",&isoElectronTracks);
  tExpectation_->Branch("isoMuonTracks",&isoMuonTracks);
  tExpectation_->Branch("isoPionTracks",&isoPionTracks);

  // di lep contribution
  tExpectation_->Branch("ExpectationDiLep",&ExpectationDiLep_);
  tExpectation_->Branch("MuDiLepControlSample",&MuDiLepControlSample_);
  tExpectation_->Branch("ElecDiLepControlSample",&ElecDiLepControlSample_);
 

  SearchBins_ = new SearchBins(true);

  if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntries();

  //  if ( max_evts > 0 && max_evts < nentries ) nentries = max_evts ;

  printf("\n\n Looping over %llu events.\n\n", nentries ) ;

  Long64_t nbytes = 0, nb = 0;

  for (Long64_t jentry=0; jentry<nentries;jentry++) {

    if ( jentry%(nentries/100) == 0 && jentry>0 ) { printf("  %9llu out of %9llu (%2lld%%)\n", jentry, nentries, ((100*jentry)/nentries)+1 ) ; fflush(stdout) ; }

    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;

    resetValues();

    if(HTgen_cut > 0.01 && genHT > HTgen_cut) continue;

    // basic kinematic cuts (probably applied at skim level)
    if(HT<500|| MHT< 200 || NJets < 7  ) continue; // NOTE: should we loosen this NJets cut for calculating efficiencies???
    if(DeltaPhi1 < 0.5 || DeltaPhi2 < 0.5 || DeltaPhi3 < 0.3 || DeltaPhi4 < 0.3) continue;
    if(!FiltersPass() ) continue;

    // bool passTrigger = false;	
    // for (std::vector<string>::iterator it = TriggerNames->begin() ; it != TriggerNames->end(); ++it){
    //   if(it->find("HLT_PFHT350_PFMET100_NoiseCleaned_v")!=std::string::npos){  // Run2015A,B
    // 	if(TriggerPass->at(it - TriggerNames->begin())>0.5) passTrigger = true;
    //   }
    //   if(it->find("HLT_PFHT350_PFMET100_JetIdCleaned_v")!=std::string::npos){  // Run2015C.D
    // 	if(TriggerPass->at(it - TriggerNames->begin())>0.5) passTrigger = true;
    //   }
    // }
    // if(useTrigger && !passTrigger) continue;

    //if(useTriggerEffWeight) Weight = Weight * GetTriggerEffWeight(MHT);

    // if(doPUreweighting){
    //   w_pu = puhist->GetBinContent(puhist->GetXaxis()->FindBin(min(TrueNumInteractions,puhist->GetBinLowEdge(puhist->GetNbinsX()+1))));
    //   Weight *= w_pu;
    // }
  
    Bin_ = SearchBins_->GetBinNumber(HT,MHT,NJets,BTags);
    //    std::cout << "Event falls in bin " << Bin_ << std::endl;
    isoTracks = isoMuonTracks + isoPionTracks + isoElectronTracks;

    GenMuNum_ = GenMus->size();
    GenElecNum_ = GenEls->size();
    GenTauNum_ = GenTaus->size();

    if (GenMuNum_+GenElecNum_<1) continue; // only want events with prompt leptons

    selectedIDMuonsNum_ = selectedIDMuons->size();
    selectedIDIsoMuonsNum_ = Muons->size();
    selectedIDElectronsNum_ = selectedIDElectrons->size();
    selectedIDIsoElectronsNum_ = Electrons->size();


    // compute efficiencies 1 muon
    if( GenMuNum_==1 && GenElecNum_==0 )
      {
	if ( GenMus->at(0).Pt() < minMuPt_ || std::abs(GenMus->at(0).Eta()) > maxMuEta_)
	  {
	    muAcc=0;
	    Expectation=1;
	    // efficiency studies TH1 and so on
	  }
	else
	  {
	    muAcc=2;
	    bool RecoNotMatched=true;
	    // efficiency studies TH1 and so on
      
	    for (UShort_t i=0; i<selectedIDMuonsNum_; i++)
	      {
		//std::cout<<"selectedIDMuonsNum_["<<i<<"] started"<<std::endl;
		if(deltaR(GenMus->at(0).Eta(),GenMus->at(0).Phi(),selectedIDMuons->at(i).Eta(),selectedIDMuons->at(i).Phi())<maxDeltaRGenToRecoMu_ && std::abs(GenMus->at(0).Pt()-selectedIDMuons->at(i).Pt())/GenMus->at(0).Pt() <maxDiffPtGenToRecoMu_)
		  {
		    // std::cout<<"selectedIDMuonsNum_["<<i<<"] started"<<std::endl;
		    RecoNotMatched=false;
		    // efficiency studies TH1 and so on
          
		    muReco =2;
		    muRecoMatched.push_back(1);
		    bool IsoNotMatched=true;
		    for (UShort_t ii=0; ii < selectedIDIsoMuonsNum_; ii++)
		      {
			if(deltaR(Muons->at(ii).Eta(),Muons->at(ii).Phi(),selectedIDMuons->at(i).Eta(),selectedIDMuons->at(i).Phi())<maxDeltaRRecoToIsoMu_ && std::abs(Muons->at(ii).Pt()-selectedIDMuons->at(i).Pt())/Muons->at(ii).Pt() <maxDiffPtRecoToIsoMu_)
			  {
			    IsoNotMatched=false;
			    // efficiency studies TH1 and so on
			    muIso=2;
			    muIsoMatched.push_back(1);
			    Expectation=2;
			    mtw = selectedIDIsoMuons_MTW->at(ii);
			    MuDiLepControlSample_=2;
			  }
			else 
			  {
			    muIsoMatched.push_back(0);
			  }
		      }
		    if(IsoNotMatched)
		      {
			muIso=0;
			Expectation=1;
		      }
		  }
		else 
		  {
		    muRecoMatched.push_back(0);
		  }
	      }
	    if(RecoNotMatched)
	      {
		muReco=0;
		Expectation=1;   
	      }
	  }
      } 
    // analyse gen electrons consider only single elec events
    //    cout << "analyse gen electrons consider only single elec events" << endl;
    if(GenMuNum_==0 && GenElecNum_==1)
      {
	if ( GenEls->at(0).Pt() < minElecPt_ || std::abs(GenEls->at(0).Eta()) > maxElecEta_)
	  {
	    elecAcc=0;
	    Expectation=1;
	  }
	else
	  {
	    elecAcc=2;
	    bool RecoNotMatched=true;

	    RecoNotMatched=true;
	    // efficiency studies TH1 and so on
	    for (UShort_t i=0; i<selectedIDElectronsNum_; i++)
	      {
		//    std::cout<<"selectedIDElectronsNum_["<<i<<"] started"<<std::endl;
		if(deltaR(GenEls->at(0).Eta(),GenEls->at(0).Phi(),selectedIDElectrons->at(i).Eta(),selectedIDElectrons->at(i).Phi())<maxDeltaRGenToRecoElec_ && std::abs(GenEls->at(0).Pt()-selectedIDElectrons->at(i).Pt())/GenEls->at(0).Pt() <maxDiffPtGenToRecoElec_)
		  {
		    //        std::cout<<"Matched to gen electron"<<std::endl;
		    RecoNotMatched=false;
		    elecReco =2;
		    elecRecoMatched.push_back(1);
		    bool IsoNotMatched=true;
		    for (UShort_t ii=0; ii < selectedIDIsoElectronsNum_; ii++)
		      {
			//      std::cout<<"selectedIDIsoElectronsNum_["<<ii<<"] started"<<std::endl;
			if(deltaR(Electrons->at(ii).Eta(),Electrons->at(ii).Phi(),selectedIDElectrons->at(i).Eta(),selectedIDElectrons->at(i).Phi())<maxDeltaRRecoToIsoElec_ && std::abs(Electrons->at(ii).Pt()-selectedIDElectrons->at(i).Pt())/Electrons->at(ii).Pt() <maxDiffPtRecoToIsoElec_)
			  {
			    IsoNotMatched=false;
			    elecIso=2;
			    elecIsoMatched.push_back(1);
			    Expectation=2;
			    mtw =  selectedIDIsoElectrons_MTW->at(ii);
			    ElecDiLepControlSample_=2;
			  }
			else 
			  {
			    elecIsoMatched.push_back(0);
			  }
		      }
		    if(IsoNotMatched)
		      {
			elecIso=0;
			Expectation=1;
		      }
		  }
		else 
		  {
		    elecRecoMatched.push_back(0);
		  }
	      }
	    if(RecoNotMatched)
	      {
		elecReco=0;
		Expectation=1;
        
		// efficiency studies TH1 and so on
	      }
	  }
      } 

    // di leptonic events
    //     cout << "Look for di leptonic events" << endl;
    if( (GenMuNum_+GenElecNum_)==2)
      {
	if(selectedIDIsoMuonsNum_==0 && selectedIDIsoElectronsNum_==0)
	  {
	    Expectation=1;
	    ExpectationDiLep_=1;
	  }
	if(selectedIDIsoMuonsNum_==1 && selectedIDIsoElectronsNum_==0)
	  {
	    mtw =  selectedIDIsoMuons_MTW->at(0);
	    MuDiLepControlSample_=0;
	  }
	if(selectedIDIsoMuonsNum_==0 && selectedIDIsoElectronsNum_==1)
	  {
	    ElecDiLepControlSample_=0;
	    mtw =  selectedIDIsoElectrons_MTW->at(0);
	  }
      }
  
   
    // ************************************************************************************************************* 22 June 2015 end****************************************************
    
    // check wether a background event according to plane lepton veto gets rejected by the isolated track veto with MT cut applied
    if(isoTracks>=1 && Expectation==1)
      {
	ExpectationReductionIsoTrack=1;
      }

    tExpectation_->Fill();
    
  } // end event loop

  TFile *outPutFile = new TFile(outputFileName.c_str(), "RECREATE"); 
  outPutFile->cd();
  tExpectation_->Write();
  outPutFile->Close();
  cout << "Saved output to " << outputFileName << endl;  
}

void ExpectationMaker::resetValues()
{
  Expectation=0;
  ExpectationReductionIsoTrack=0;
  muIso =1;
  muReco =1;
  muAcc =1;
  muMTW =1;
  muTotal=1;
  elecIso =1;
  elecReco =1;
  elecAcc =1;
  elecTotal=1;
  elecMTW=1;
  // di lep
  ExpectationDiLep_=0;
  MuDiLepControlSample_=1;
  ElecDiLepControlSample_=1;

  muIsoMatched.clear();
  muRecoMatched.clear();
  elecIsoMatched.clear();
  elecRecoMatched.clear();
  mtw=-1.;
}

bool ExpectationMaker::FiltersPass()
{
  bool result=true;
  //if(useFilterData){
    //if(!CSCTightHaloFilter) result=false;
    if(eeBadScFilter!=1) result=false;
    if(!eeBadSc4Filter) result=false;
    if(!HBHENoiseFilter) result=false;
    if(!HBHEIsoNoiseFilter) result=false;
    //  }
  if(NVtx<=0) result=false;
  // Do not apply on fastSim samples!
  // if(!signalScan) if(JetID!=1) result=false;
  return result;
}
