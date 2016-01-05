#include<iostream>
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include "TLorentzVector.h"
#include<vector>
#include "TTree.h"
using namespace std;

class ReadTree {

public :

   TTree          *fChain;   //!pointer to the analyzed TTree or TChain

   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   Int_t           Eventno;
   Double_t        HT;
   Double_t        MHT;
   Int_t           NJets;
   Int_t           BJets;
   Double_t        dPhi1;
   Double_t        dPhi2;
   Double_t        dPhi3;
   Double_t        dPhi4;
   Double_t        evWeight;
   
   Int_t           IchSR;
   Int_t           IchSB;
   Int_t           isBarrel;
   Int_t           isEndcap;
   Int_t           nonPrompt;
   Double_t        SigmaIetaIeta;
   Double_t        photonPt;
   Double_t        photonEta;
   
   vector<string>  *TriggerNames;
   vector<bool>    *TriggerPass;
   // List of branches
   
    TBranch        *b_Eventno;   //!
   TBranch        *b_HT;   //!
   TBranch        *b_MHT;   //!
   TBranch        *b_NJets;   //!
   TBranch        *b_BJets;   //!
   TBranch        *b_dPhi1;   //!
   TBranch        *b_dPhi2;   //!
   TBranch        *b_dPhi3;   //!
   TBranch        *b_dPhi4;   //!
   TBranch        *b_evWeight;   //!
   TBranch        *b_nonPrompt;   //!
   
   TBranch        *b_IchSR;   //!
   TBranch        *b_IchSB;   //!
   TBranch        *b_isBarrel;   //!
   TBranch        *b_isEndcap;   //!
   TBranch        *b_SigmaIetaIeta;   //!
   TBranch        *b_photonPt;   //!
   TBranch        *b_photonEta;   //!
   TBranch        *b_TriggerNames;   //!
   TBranch        *b_TriggerPass;   //!

   ReadTree(TTree *tree);

   virtual ~ReadTree();
   bool BaseLine();

   bool MHTlow();
   bool MHThigh();
   

};

ReadTree::~ReadTree()

{

   if (!fChain) return;

   delete fChain->GetCurrentFile();

}

 ReadTree::ReadTree(TTree *tree)

{


fChain=tree;
   

  

   TriggerNames = 0;
   TriggerPass = 0;
   
   fChain->SetBranchAddress("Eventno", &Eventno, &b_Eventno);
   fChain->SetBranchAddress("HT", &HT, &b_HT);
   fChain->SetBranchAddress("MHT", &MHT, &b_MHT);
   fChain->SetBranchAddress("NJets", &NJets, &b_NJets);
   fChain->SetBranchAddress("BJets", &BJets, &b_BJets);
   fChain->SetBranchAddress("dPhi1", &dPhi1, &b_dPhi1);
   fChain->SetBranchAddress("dPhi2", &dPhi2, &b_dPhi2);
   fChain->SetBranchAddress("dPhi3", &dPhi3, &b_dPhi3);
   fChain->SetBranchAddress("dPhi4", &dPhi4, &b_dPhi4);
   fChain->SetBranchAddress("evWeight", &evWeight, &b_evWeight);
   
   fChain->SetBranchAddress("IchSR", &IchSR, &b_IchSR);
   fChain->SetBranchAddress("IchSB", &IchSB, &b_IchSB);
   fChain->SetBranchAddress("isBarrel", &isBarrel, &b_isBarrel);
   fChain->SetBranchAddress("isEndcap", &isEndcap, &b_isEndcap);
   fChain->SetBranchAddress("SigmaIetaIeta", &SigmaIetaIeta, &b_SigmaIetaIeta);
   fChain->SetBranchAddress("photonPt", &photonPt, &b_photonPt);
   fChain->SetBranchAddress("photonEta", &photonEta, &b_photonEta);
   fChain->SetBranchAddress("nonPrompt", &nonPrompt, &b_nonPrompt);
   fChain->SetBranchAddress("TriggerNames", &TriggerNames, &b_TriggerNames);
   fChain->SetBranchAddress("TriggerPass", &TriggerPass, &b_TriggerPass);

}





bool ReadTree::MHTlow(){

if(NJets >=4  && HT > 500. && MHT >200. && MHT <500 && dPhi1>0.5 && dPhi2>0.5 && dPhi3 > 0.3 && dPhi4 > 0.3){
return true;
}else{

return false;
}

}








bool ReadTree::MHThigh(){

if(NJets >=4  && HT > 500. && MHT >500. && dPhi1>0.5 && dPhi2>0.5 && dPhi3 > 0.3 && dPhi4 > 0.3){
return true;
}else{

return false;
}

}





bool ReadTree::BaseLine(){//baseline





if(NJets >=4  && HT > 500. && MHT >200.  && dPhi1>0.5 && dPhi2>0.5 && dPhi3 > 0.3 && dPhi4> 0.3){



return true;
}else{

return false;
}



}//baseline




 
