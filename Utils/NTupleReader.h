#ifndef STOP_NTUPLE_READER_H
#define STOP_NTUPLE_READER_H

#include "TFile.h"
#include "TBranch.h"
#include "TTree.h"
#include "TLorentzVector.h"

#include "TStopwatch.h"

#include <vector>
#include <cstdio>
#include <map>
#include <string>
#include <vector>

/* This class is designed to be a simple interface to reading stop NTuples
   
   To use this class simply open the desired Tree as a TTree or TChain and pass it 
   to the constructor.  Then the tuple contents may be accessed as follows

   NTupleReader tr(tree);
   while(tr.getNextEvent())
   {
       int run = tr.run;
   }
   
   
   To add a variable to the reader it should be declared in the public section of the class definition,
   and added to the branchMap_ variable in the populateBranchList() map.  Vectors they must be devlared
   as pointers and initialized to zero in the constructor but not in clearTuple().  For simple variables 
   they shoud be initialized to an approperiate value in clearTuple().  
 */

//                                    minAbsEta, maxAbsEta, minPt, maxPt
static const double      pt30Arr[] = {   -1,        -1,      30,    -1  };
static const double pt30Eta25Arr[] = {   -1,       2.5,      30,    -1  };
static const double pt50Eta25Arr[] = {   -1,       2.5,      50,    -1  };
static const double pt70Eta25Arr[] = {   -1,       2.5,      70,    -1  };
static const double      dphiArr[] = {   -1,       5.0,      30,    -1  };

static const double expectedLumi = 5000; // in pb-1 --> equivalent to 5 fb-1

TStopwatch timer;

using namespace std;

class NTupleReader
{

public:
    // List of all variables used in tuple
    unsigned int run, lumi, event;
    double mht, mhtphi, ht, met, metphi, dPhi0_CUT, dPhi1_CUT, dPhi2_CUT, true_npv, avg_npv;
    double bestTopJetMass, MT2, mTbestTopJet, mTbJet, linearCombmTbJetPlusmTbestTopJet;
    double mTbestWJet, mTbestbJet, mTremainingTopJet, evtWeight;
    int nMuons_CUT, nMuons, nElectrons_CUT, nElectrons, nJets, loose_nIsoTrks, nIsoTrks_CUT;
    int nJets_CUT, vtxSize, npv, nm1, n0, np1, bestTopJetIdx, pickedRemainingCombfatJetIdx;
    unsigned int remainPassCSVS;
    std::vector<double> *muonsCharge, *muonsMtw, *muonsRelIso, *elesCharge, *elesMtw, *elesRelIso;
    std::vector<double> *recoJetsBtag_0, *trksForIsoVeto_charge, *trksForIsoVeto_dz;
    std::vector<double> *loose_isoTrks_charge, *loose_isoTrks_dz;
    std::vector<double> *loose_isoTrks_iso, *loose_isoTrks_mtw;
    std::vector<int> *recoJetsFlavor, *genDecayIdxVec, *genDecayPdgIdVec, *genDecayMomIdxVec;
    std::vector<int> *W_emuVec, *W_tau_emuVec, *W_tau_prongsVec, *trksForIsoVeto_pdgId;
    std::vector<int> *trksForIsoVeto_idx, *loose_isoTrks_pdgId, *loose_isoTrks_idx;
    std::vector<int> *forVetoIsoTrksidx, *genDecayStrVec;
    std::vector<TLorentzVector> *muonsLVec, *elesLVec, *jetsLVec, *genDecayLVec;
    std::vector<TLorentzVector> *trksForIsoVetoLVec, *loose_isoTrksLVec;

    NTupleReader(TTree * tree);

    int getNEntries()
    {
        if(tree_ && totEntries_ == -1 ) totEntries_ = tree_->GetEntries();
        else if( !tree_ )
        {
            printf("NTupleReader::getNEntries() - NO tree defined yet!!!\n");
            return -1;
        }
        return totEntries_;
    }

    bool getNextEvent();
    int getNevtProcessed(){ return nevt_; }

    double calcHT(const vector<TLorentzVector> &inputJets, const double *jetCutsArr);
    double calcMHT(const vector<TLorentzVector> &inputJets, const double *jetCutsArr);
    vector<double> calcMHTxy(const vector<TLorentzVector> &inputJets, const double *jetCutsArr);

    double calcHT(){ return calcHT((*jetsLVec), pt50Eta25Arr); }
    double calcMHT(){ return calcMHT((*jetsLVec), pt30Arr); }
    vector<double> calcMHTxy(){ return calcMHTxy((*jetsLVec), pt30Arr); }

    int countJets(const vector<TLorentzVector> &inputJets, const double *jetCutsArr);
    int countJets(){ return countJets((*jetsLVec), pt50Eta25Arr); }

    vector<double> calcDPhi(const vector<TLorentzVector> &inputJets, const double tmpmetphi, const int nDPhi, const double *jetCutsArr);

    void setStartEvtIdx(int startIdx){ nevt_ = startIdx; }

private:
    // private variabals for internal use
    TTree *tree_;
    int nevt_;

    int totEntries_;

    // Map to hold branch list 
    std::map<std::string, void *> branchMap_;

    void activateBranches();
    void populateBranchList();
    void clearTuple();
};

NTupleReader::NTupleReader(TTree * tree)
{
    tree_ = tree;
    nevt_ = 0;
    totEntries_ = -1;
    clearTuple();
    
    //vectors must be initialized to 0 here to avoid segfaults.  This cannot be done 
    //in clearTuple() as it will cause memory leaks.
    muonsCharge = muonsMtw = muonsRelIso = elesCharge = elesMtw = elesRelIso = 0;
    recoJetsBtag_0 = trksForIsoVeto_charge = trksForIsoVeto_dz = 0;
    loose_isoTrks_charge = loose_isoTrks_dz = 0;
    loose_isoTrks_iso = loose_isoTrks_mtw = 0;
    recoJetsFlavor = genDecayIdxVec = genDecayPdgIdVec = genDecayMomIdxVec = 0;
    W_emuVec = W_tau_emuVec = W_tau_prongsVec = trksForIsoVeto_pdgId = 0;
    trksForIsoVeto_idx = loose_isoTrks_pdgId = loose_isoTrks_idx = 0;
    forVetoIsoTrksidx = genDecayStrVec = 0;
    muonsLVec = elesLVec = jetsLVec = genDecayLVec = 0;
    trksForIsoVetoLVec = loose_isoTrksLVec = 0;

    activateBranches();
}

void NTupleReader::activateBranches()
{
    tree_->SetBranchStatus("*", 0);

    // Add desired branches to branchMap_
    populateBranchList();

    for(std::map<std::string, void *>::const_iterator iMap = branchMap_.begin(); iMap != branchMap_.end(); ++iMap)
    {
        tree_->SetBranchStatus(iMap->first.c_str(), 1);
        tree_->SetBranchAddress(iMap->first.c_str(), iMap->second);
    }
}

void NTupleReader::populateBranchList()
{
    branchMap_["run"]   = &run;
    branchMap_["event"] = &event;
    branchMap_["lumi"]  = &lumi;   
    branchMap_["evtWeight"]  = &evtWeight;
    branchMap_["vtxSize"]  = &vtxSize;
    branchMap_["muonsRelIso"]  = &muonsRelIso;
    branchMap_["elesRelIso"]  = &elesRelIso;
    branchMap_["genDecayIdxVec"]  = &genDecayIdxVec;
    branchMap_["genDecayPdgIdVec"]  = &genDecayPdgIdVec;
    branchMap_["genDecayMomIdxVec"]  = &genDecayMomIdxVec;
    branchMap_["W_emuVec"]  = &W_emuVec;
    branchMap_["W_tau_emuVec"]  = &W_tau_emuVec;
    branchMap_["W_tau_prongsVec"]  = &W_tau_prongsVec;
    branchMap_["muonsLVec"]  = &muonsLVec;
    branchMap_["elesLVec"]  = &elesLVec;
    branchMap_["jetsLVec"]  = &jetsLVec;
    branchMap_["genDecayLVec"]  = &genDecayLVec;
}

bool NTupleReader::getNextEvent()
{
    if(nevt_ >= getNEntries())
    {
        return false;
    }
    clearTuple();
    int status = tree_->GetEntry(nevt_);
    nevt_++;
    return status > 0;
}

void NTupleReader::clearTuple()
{
    run = lumi = event = -1;
    mht = mhtphi = ht = met = metphi = dPhi0_CUT = dPhi1_CUT = dPhi2_CUT = true_npv = avg_npv = 0.0;
    bestTopJetMass = MT2 = mTbestTopJet = mTbJet = linearCombmTbJetPlusmTbestTopJet = 0.0;
    mTbestWJet = mTbestbJet = mTremainingTopJet = evtWeight = 0.0;
    nMuons_CUT = nMuons = nElectrons_CUT = nElectrons = nJets = loose_nIsoTrks = nIsoTrks_CUT = 0;
    nJets_CUT = vtxSize = npv = nm1 = n0 = np1 = bestTopJetIdx = pickedRemainingCombfatJetIdx = 0;
    remainPassCSVS = 0;
}

int NTupleReader::countJets(const vector<TLorentzVector> &inputJets, const double *jetCutsArr){

   const double minAbsEta = jetCutsArr[0], maxAbsEta = jetCutsArr[1], minPt = jetCutsArr[2], maxPt = jetCutsArr[3];

   int cntNJets =0;
   for(unsigned int ij=0; ij<inputJets.size(); ++ij){
      double perjetpt = inputJets[ij].Pt(), perjeteta = inputJets[ij].Eta();
      if(   ( minAbsEta == -1 || std::abs(perjeteta) >= minAbsEta )
         && ( maxAbsEta == -1 || std::abs(perjeteta) < maxAbsEta )
         && (     minPt == -1 || perjetpt >= minPt )
         && (     maxPt == -1 || perjetpt < maxPt ) ){
         cntNJets ++;
      }
   }
   return cntNJets;
}

vector<double> NTupleReader::calcDPhi(const vector<TLorentzVector> &inputJets, const double tmpmetphi, const int nDPhi, const double *jetCutsArr){

   const double minAbsEta = jetCutsArr[0], maxAbsEta = jetCutsArr[1], minPt = jetCutsArr[2], maxPt = jetCutsArr[3];

   int cntNJets =0;
   vector<double> outDPhiVec(nDPhi, 999);
   for(unsigned int ij=0; ij<inputJets.size(); ++ij){
      double perjetpt = inputJets[ij].Pt(), perjeteta = inputJets[ij].Eta();
      if(   ( minAbsEta == -1 || std::abs(perjeteta) >= minAbsEta )
         && ( maxAbsEta == -1 || std::abs(perjeteta) < maxAbsEta )
         && (     minPt == -1 || perjetpt >= minPt )
         && (     maxPt == -1 || perjetpt < maxPt ) ){

         if( cntNJets < nDPhi ){
            double perDPhi = std::abs(TVector2::Phi_mpi_pi( inputJets[ij].Phi() - tmpmetphi ));
            outDPhiVec[cntNJets] = perDPhi;
         }
         ++ cntNJets;
      }
   }
   return outDPhiVec;
}

double NTupleReader::calcHT(const vector<TLorentzVector> &inputJets, const double *jetCutsArr){
   double HT = 0;
   for(unsigned int ij=0; ij<inputJets.size(); ++ij){
      vector<TLorentzVector> dummyVec; dummyVec.push_back(inputJets.at(ij));
      if( !countJets(dummyVec, jetCutsArr) ) continue;
      HT += inputJets.at(ij).Pt();
   }
   return HT;
}

vector<double> NTupleReader::calcMHTxy(const vector<TLorentzVector> &inputJets, const double *jetCutsArr){
   double MHTx = 0, MHTy = 0;
   for(unsigned int ij=0; ij<inputJets.size(); ++ij){
      vector<TLorentzVector> dummyVec; dummyVec.push_back(inputJets.at(ij));
      if( !countJets(dummyVec, jetCutsArr) ) continue;
      MHTx -= inputJets.at(ij).Px();
      MHTy -= inputJets.at(ij).Py();
   }
   vector<double> xyMHT;
   xyMHT.push_back(MHTx); xyMHT.push_back(MHTy);
   return xyMHT;
}

double NTupleReader::calcMHT(const vector<TLorentzVector> &inputJets, const double *jetCutsArr){
   double MHTx = 0, MHTy = 0;
   for(unsigned int ij=0; ij<inputJets.size(); ++ij){
      vector<TLorentzVector> dummyVec; dummyVec.push_back(inputJets.at(ij));
      if( !countJets(dummyVec, jetCutsArr) ) continue;
      MHTx -= inputJets.at(ij).Px();
      MHTy -= inputJets.at(ij).Py();
   }
   return sqrt( MHTx*MHTx + MHTy*MHTy );
}

#endif
