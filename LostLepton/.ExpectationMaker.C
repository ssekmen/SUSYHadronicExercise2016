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
  tExpectation_->Branch("elecAcc",&elecAcc);  
  tExpectation_->Branch("elecReco",&elecReco);  
  tExpectation_->Branch("elecIso",&elecIso);   
  tExpectation_->Branch("selectedIDIsoMuonsNum",&selectedIDIsoMuonsNum_);
  tExpectation_->Branch("selectedIDIsoMuons", "std::vector<TLorentzVector>", &Muons, 32000, 0);
  tExpectation_->Branch("selectedIDIsoMuons_MTW", &selectedIDIsoMuons_MTW);
  tExpectation_->Branch("selectedIDIsoMuons_MT2Activity", &selectedIDIsoMuons_MT2Activity);
  tExpectation_->Branch("selectedIDMuonsNum",&selectedIDMuonsNum_);
  tExpectation_->Branch("selectedIDMuons", "std::vector<TLorentzVector>", &selectedIDMuons, 32000, 0);
  tExpectation_->Branch("selectedIDMuons_MTW", &selectedIDMuons_MTW);
  tExpectation_->Branch("selectedIDMuons_MiniIso", &selectedIDMuons_MiniIso);
  tExpectation_->Branch("selectedIDIsoElectronsNum",&selectedIDIsoElectronsNum_);
  tExpectation_->Branch("selectedIDIsoElectrons", "std::vector<TLorentzVector>", &Electrons, 32000, 0);
  tExpectation_->Branch("selectedIDIsoElectrons_MTW", &selectedIDIsoElectrons_MTW);
  tExpectation_->Branch("selectedIDIsoElectrons_MT2Activity", &selectedIDIsoElectrons_MT2Activity);
  tExpectation_->Branch("selectedIDElectronsNum",&selectedIDElectronsNum_);
  tExpectation_->Branch("selectedIDElectrons", "std::vector<TLorentzVector>", &selectedIDElectrons, 32000, 0);
  tExpectation_->Branch("selectedIDElectrons_MTW", &selectedIDElectrons_MTW);
  tExpectation_->Branch("selectedIDElectrons_MiniIso", &selectedIDElectrons_MiniIso);
 
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

    // for the leptonic ttbar smaples inclusive in genHT, we'll reject high-genHT events to avoid double-counting
    if(HTgen_cut > 0.01 && genHT > HTgen_cut) continue;

    // basic kinematic cuts (probably applied at skim level)
    if(HT<500|| MHT< 200 || NJets < 7  ) continue; // something to ponder: should we loosen this NJets cut for calculating efficiencies???
    if(DeltaPhi1 < 0.5 || DeltaPhi2 < 0.5 || DeltaPhi3 < 0.3 || DeltaPhi4 < 0.3) continue;
    // event cleaning -- reject events with bad detector effects, etc.
    if(!FiltersPass() ) continue;

    // we don't apply the simulated trigger on the MC, but we doapply a fudge-factor to account for small inneficiency at low-MET
    // trigger weights hard-coded in LLTools.h
    if(useTriggerEffWeight) Weight = Weight * GetTriggerEffWeight(MHT);
  
    Bin_ = SearchBins_->GetBinNumber(HT,MHT,NJets,BTags);
    isoTracks = isoMuonTracks + isoPionTracks + isoElectronTracks;

    GenMuNum_ = GenMus->size();
    GenElecNum_ = GenEls->size();
    GenTauNum_ = GenTaus->size();

    if (GenMuNum_+GenElecNum_<1) continue; // only want events with prompt leptons

    selectedIDMuonsNum_ = selectedIDMuons->size();
    selectedIDIsoMuonsNum_ = Muons->size();
    selectedIDElectronsNum_ = selectedIDElectrons->size();
    selectedIDIsoElectronsNum_ = Electrons->size();


    // compute variables useful for calculating muon efficiencies
    // Expectation can be 1 (lost lepton event), 2 (lepton passes all cuts, not lost)
    // muAcc, muReco, muIso can be 0 (muon fails cut), 2 (muon passed cut), or 1 (no relevant muon in event)
    // also will define MuDiLepControlSample -- what does this do?
    if( GenMuNum_==1 && GenElecNum_==0 )
      { // true single-muon events
	if ( GenMus->at(0).Pt() < minMuPt_ || std::abs(GenMus->at(0).Eta()) > maxMuEta_) // is the muon in kinematic and geometric acceptance?
	  { // not in acceptance -- muon is lost
	    muAcc=0;
	    Expectation=1;
	    // efficiency studies TH1 and so on
	  }
	else
	  { // muon is in acceptance, now check if it was recosntructed and ID'd
	    muAcc=2;
	    bool RecoNotMatched=true;
	    for (UShort_t i=0; i<selectedIDMuonsNum_; i++) // loop over the RECO muons, try to find one matching the gen muon
	      {
		// deltaR = sqrt(dEta*dEta + dPhi*dPhi)
		if(deltaR(GenMus->at(0).Eta(),GenMus->at(0).Phi(),selectedIDMuons->at(i).Eta(),selectedIDMuons->at(i).Phi())<maxDeltaRGenToRecoMu_ && std::abs(GenMus->at(0).Pt()-selectedIDMuons->at(i).Pt())/GenMus->at(0).Pt() <maxDiffPtGenToRecoMu_)
		  { // found a RECO muon mathching the gen particle -- the muon was reconstructed, now check to see if it passes the isolation cut
		    RecoNotMatched=false;          
		    muReco =2;
		    bool IsoNotMatched=true;
		    for (UShort_t ii=0; ii < selectedIDIsoMuonsNum_; ii++) // loop over the isolated muons (also RECO'd and ID'd), try to find another match
		      {
			if(deltaR(Muons->at(ii).Eta(),Muons->at(ii).Phi(),selectedIDMuons->at(i).Eta(),selectedIDMuons->at(i).Phi())<maxDeltaRRecoToIsoMu_ && std::abs(Muons->at(ii).Pt()-selectedIDMuons->at(i).Pt())/Muons->at(ii).Pt() <maxDiffPtRecoToIsoMu_)
			  { // muon was also isolated -- we found it
			    IsoNotMatched=false;
			    muIso=2;
			    Expectation=2;
			    mtw = selectedIDIsoMuons_MTW->at(ii); // compute the transverse mass of the mu-MET system
			    MuDiLepControlSample_=2;
			  }
		      }
		    if(IsoNotMatched)
		      { // we couldn't find any isolated muon match -- the muon failed the isolation cut
			muIso=0;
			Expectation=1; // lost-lepton event
		      }
		  }
	      }
	    if(RecoNotMatched)
	      { // we couldn't find any reconstructed lepton
		muReco=0;
		Expectation=1; // lost-lepton event  
	      }
	  }
      } 

    // now compute variables useful for calculating electron efficiencies
    // Expectation can be 1 (lost lepton event), 2 (lepton passes all cuts, not lost)
    // elecAcc, elecReco, elecIso can be 0 (electron fails cut), 2 (electron passed cut), or 1 (no relevant electron in event)
    if( GenMuNum_==0 && GenElecNum_==1 )
      { // true single-electron events
	if ( GenEls->at(0).Pt() < minElecPt_ || std::abs(GenEls->at(0).Eta()) > maxElecEta_) // is the electron in kinematic and geometric acceptance?
	  { // not in acceptance -- electron is lost
	    elecAcc=0;
	    Expectation=1;
	    // efficiency studies TH1 and so on
	  }
	else
	  { // electron is in acceptance, now check if it was recosntructed and ID'd
	    elecAcc=2;
	    bool RecoNotMatched=true;
	    for (UShort_t i=0; i<selectedIDElectronsNum_; i++) // loop over the RECO electrons, try to find one matching the gen electron
	      {
		// deltaR = sqrt(dEta*dEta + dPhi*dPhi)
		if(deltaR(GenEls->at(0).Eta(),GenEls->at(0).Phi(),selectedIDElectrons->at(i).Eta(),selectedIDElectrons->at(i).Phi())<maxDeltaRGenToRecoElec_ && std::abs(GenEls->at(0).Pt()-selectedIDElectrons->at(i).Pt())/GenEls->at(0).Pt() <maxDiffPtGenToRecoElec_)
		  { // found a RECO electron mathching the gen particle -- the electron was reconstructed, now check to see if it passes the isolation cut
		    RecoNotMatched=false;          
		    elecReco =2;
		    bool IsoNotMatched=true;
		    for (UShort_t ii=0; ii < selectedIDIsoElectronsNum_; ii++) // loop over the isolated electrons (also RECO'd and ID'd), try to find another match
		      {
			if(deltaR(Electrons->at(ii).Eta(),Electrons->at(ii).Phi(),selectedIDElectrons->at(i).Eta(),selectedIDElectrons->at(i).Phi())<maxDeltaRRecoToIsoElec_ && std::abs(Electrons->at(ii).Pt()-selectedIDElectrons->at(i).Pt())/Electrons->at(ii).Pt() <maxDiffPtRecoToIsoElec_)
			  { // electron was also isolated -- we found it
			    IsoNotMatched=false;
			    elecIso=2;
			    Expectation=2;
			    mtw = selectedIDIsoElectrons_MTW->at(ii); // compute the transverse mass of the elec-MET system
			    ElecDiLepControlSample_=2;
			  }
		      }
		    if(IsoNotMatched)
		      { // we couldn't find any isolated electron match -- the electron failed the isolation cut
			elecIso=0;
			Expectation=1; // lost-lepton event
		      }
		  }
	      }
	    if(RecoNotMatched)
	      { // we couldn't find any reconstructed lepton
		elecReco=0;
		Expectation=1; // lost-lepton event  
	      }
	  }
      } 

    // di--leptonic events -- a very small BG 
    // they also can contaminate our one-lepton control region
    // we'll find a small fudge factor to account for both of these effects
    if( (GenMuNum_+GenElecNum_)==2)
      { // true di-lepton event
	if(selectedIDIsoMuonsNum_==0 && selectedIDIsoElectronsNum_==0)
	  { // completely lost
	    Expectation=1;
	    ExpectationDiLep_=1;
	  }
	if(selectedIDIsoMuonsNum_==1 && selectedIDIsoElectronsNum_==0)
	  { // contaminates 1-muon region
	    mtw =  selectedIDIsoMuons_MTW->at(0);
	    MuDiLepControlSample_=0;
	  }
	if(selectedIDIsoMuonsNum_==0 && selectedIDIsoElectronsNum_==1)
	  { // contaminates 1-electron region
	    ElecDiLepControlSample_=0;
	    mtw =  selectedIDIsoElectrons_MTW->at(0);
	  }
      }
  
  
    // to this point we have neglected the isolated track veto
    // calculate probability that a background event that we did not reject with the standard lepton veto will be rejected by the track veto
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

  mtw=-1.;
}

bool ExpectationMaker::FiltersPass()
{
  bool result=true;

  if(!CSCTightHaloFilter) result=false;
  if(eeBadScFilter!=1) result=false;
  if(!eeBadSc4Filter) result=false;
  if(!HBHENoiseFilter) result=false;
  if(!HBHEIsoNoiseFilter) result=false;

  if(NVtx<=0) result=false;
  
  return result;
}
