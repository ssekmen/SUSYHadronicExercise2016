#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <iomanip>
#include <locale>
#include <sstream>

#include "TTree.h"
#include "TFile.h"
#include "TChain.h"

#include "TString.h"
#include "TLorentzVector.h"

#include "TInterpreter.h"

//                                    minAbsEta, maxAbsEta, minPt, maxPt
static const double      pt30Arr[] = {   -1,        -1,      30,    -1  };
static const double pt30Eta25Arr[] = {   -1,       2.5,      30,    -1  };
static const double pt50Eta25Arr[] = {   -1,       2.5,      50,    -1  };
static const double pt70Eta25Arr[] = {   -1,       2.5,      70,    -1  };
static const double      dphiArr[] = {   -1,       5.0,      30,    -1  };

int countJets(const vector<TLorentzVector> &inputJets, const double *jetCutsArr){

   const double minAbsEta = jetCutsArr[0], maxAbsEta = jetCutsArr[1], minPt = jetCutsArr[2], maxPt = jetCutsArr[3];

   int cntNJets =0;
   for(unsigned int ij=0; ij<inputJets.size(); ++ij){
      double perjetpt = inputJets[ij].Pt(), perjeteta = inputJets[ij].Eta();
      if(   ( minAbsEta == -1 || fabs(perjeteta) >= minAbsEta )
         && ( maxAbsEta == -1 || fabs(perjeteta) < maxAbsEta )
         && (     minPt == -1 || perjetpt >= minPt )
         && (     maxPt == -1 || perjetpt < maxPt ) ){
         cntNJets ++;
      }
   }
   return cntNJets;
}

double calcHT(const vector<TLorentzVector> &inputJets, const double *jetCutsArr){
   double HT = 0;
   for(unsigned int ij=0; ij<inputJets.size(); ++ij){
      vector<TLorentzVector> dummyVec; dummyVec.push_back(inputJets.at(ij));
      if( !countJets(dummyVec, jetCutsArr) ) continue;
      HT += inputJets.at(ij).Pt();
   }
   return HT;
}

void slimming(std::string filelist="rootlist.txt", std::string treeName="stopTreeMaker/AUX"){

   gInterpreter->GenerateDictionary("vector<TLorentzVector>","TLorentzVector.h;vector");

   std::vector<std::string> cachedFilesVec;
   std::map<std::string, int> fileIdxMap;

   unsigned int run, lumi, event;
   double evtWeight;
   int vtxSize;
   std::vector<double> *muonsRelIso =0, *elesRelIso =0;
   std::vector<int> *genDecayIdxVec =0, *genDecayPdgIdVec =0, *genDecayMomIdxVec =0;
   std::vector<int> *W_emuVec =0, *W_tau_emuVec =0, *W_tau_prongsVec =0;
   std::vector<TLorentzVector> *muonsLVec =0, *elesLVec =0, *jetsLVec =0, *genDecayLVec =0;

   char filename[500];
   TString filenameT; 
 
   ifstream fin(filelist.c_str());
   while(fin.getline(filename, 500)){
      std::cout<<"\nProcessing the "<<filename<<" file ..."<<std::endl;

      TChain  *chn = new TChain(treeName.c_str());
      chn->Add(filename);
      filenameT = filename;

      TObjArray * vlist =0;
      vlist = filenameT.Tokenize("/.");
      TObjString* obj = dynamic_cast<TObjString*>(vlist->At(vlist->GetEntries()-2));
      TString outputT = obj->GetString();

      int currExtIdx = 1;
      if( fileIdxMap.find(outputT.Data()) != fileIdxMap.end() ){
         currExtIdx = fileIdxMap[outputT.Data()];
      }else{
         fileIdxMap[outputT.Data()] = currExtIdx;
      }

      std::vector<std::string>::iterator iter = std::find(cachedFilesVec.begin(), cachedFilesVec.end(), outputT.Data());
      if( iter != cachedFilesVec.end() ){
         currExtIdx ++;
         fileIdxMap[outputT.Data()] = currExtIdx;
         ostringstream convert;
         convert << currExtIdx;
         outputT = outputT+"_"+convert.str();
      }else{
         cachedFilesVec.push_back(outputT.Data());
      }
      std::cout<<"  --> outputT : "<<outputT<<std::endl;

      chn->SetBranchStatus("*", 0);
   
      chn->SetBranchStatus("run", 1); chn->SetBranchAddress("run", &run);
      chn->SetBranchStatus("lumi", 1); chn->SetBranchAddress("lumi", &lumi); 
      chn->SetBranchStatus("event", 1); chn->SetBranchAddress("event", &event);
      chn->SetBranchStatus("evtWeight", 1); chn->SetBranchAddress("evtWeight", &evtWeight);
      chn->SetBranchStatus("vtxSize", 1); chn->SetBranchAddress("vtxSize", &vtxSize);
      chn->SetBranchStatus("muonsRelIso", 1); chn->SetBranchAddress("muonsRelIso", &muonsRelIso);
      chn->SetBranchStatus("elesRelIso", 1); chn->SetBranchAddress("elesRelIso", &elesRelIso);
      chn->SetBranchStatus("genDecayIdxVec", 1); chn->SetBranchAddress("genDecayIdxVec", &genDecayIdxVec);
      chn->SetBranchStatus("genDecayPdgIdVec", 1); chn->SetBranchAddress("genDecayPdgIdVec", &genDecayPdgIdVec);
      chn->SetBranchStatus("genDecayMomIdxVec", 1); chn->SetBranchAddress("genDecayMomIdxVec", &genDecayMomIdxVec);
      chn->SetBranchStatus("W_emuVec", 1); chn->SetBranchAddress("W_emuVec", &W_emuVec);
      chn->SetBranchStatus("W_tau_emuVec", 1); chn->SetBranchAddress("W_tau_emuVec", &W_tau_emuVec);
      chn->SetBranchStatus("W_tau_prongsVec", 1); chn->SetBranchAddress("W_tau_prongsVec", &W_tau_prongsVec);
      chn->SetBranchStatus("muonsLVec", 1); chn->SetBranchAddress("muonsLVec", &muonsLVec);
      chn->SetBranchStatus("elesLVec", 1); chn->SetBranchAddress("elesLVec", &elesLVec);
      chn->SetBranchStatus("jetsLVec", 1); chn->SetBranchAddress("jetsLVec", &jetsLVec);
      chn->SetBranchStatus("genDecayLVec", 1); chn->SetBranchAddress("genDecayLVec", &genDecayLVec);
   
      TTree *newtree = chn->CloneTree(0);
   
      TFile *newfile = new TFile("slimmed_"+outputT+".root", "recreate");
      TDirectory *newdir = newfile->mkdir("stopTreeMaker");
      newdir->cd();
   
      int nEntries = chn->GetEntries();
      for(int ie=0; ie<nEntries; ie++){
         chn->GetEntry(ie);
   
         int selNJets = countJets((*jetsLVec), pt50Eta25Arr);
         double selHT = calcHT((*jetsLVec), pt50Eta25Arr);
   
         if( selNJets < 3 ) continue;
         if( selHT < 300 ) continue;
   
         newtree->Fill();
      }
      newtree->Write();
      newfile->Write(); newfile->Close();

      if (chn) delete chn;
   }

}
