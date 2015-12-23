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
    Int_t           NVtx;
    Int_t           eeBadScFilter;
    Bool_t          eeBadSc4Filter;
    Bool_t          HBHENoiseFilter;
    Bool_t          HBHEIsoNoiseFilter;
    UInt_t          RunNum;
    UInt_t          LumiBlockNum;
    ULong64_t       EvtNum;
    double         HT;
    double         MHT;
    int            NJets;
    int            BTags;
    double         DeltaPhi1clean;
    double         DeltaPhi2clean;
    double         DeltaPhi3clean;
    double         Weight;
    double        puWeight;
    vector<double>  *photon_sigmaIetaIeta;
    vector<double>  *photon_pfChargedIsoRhoCorr;
    vector<bool>    *photon_nonPrompt;
    vector<TLorentzVector> *photonCands;
    vector<TLorentzVector> *bestPhoton;
    vector<string>  *TriggerNames;
    vector<bool>    *TriggerPass;
    vector<int>     *TriggerPrescales;

    TBranch        *b_NVtx;   //!
    TBranch        *b_eeBadScFilter;
    TBranch        *b_eeBadSc4Filter;
    TBranch        *b_HBHENoiseFilter;
    TBranch        *b_HBHEIsoNoiseFilter;
    TBranch        *b_RunNum;   //!
    TBranch        *b_LumiBlockNum;   //!
    TBranch        *b_EvtNum;   // 
    TBranch        *b_HT;   //!
    TBranch        *b_MHT;   //!
    TBranch        *b_NJets;   //!
    TBranch        *b_BTags;   //!
    TBranch        *b_DeltaPhi1clean;   //!
    TBranch        *b_DeltaPhi2clean;   //!
    TBranch        *b_DeltaPhi3clean;   //!
    TBranch        *b_Weight;   //!
    TBranch        *b_puWeight;   //!
    TBranch        *b_photonCands;   //!
    TBranch        *b_bestPhoton;   //!
    TBranch        *b_photon_sigmaIetaIeta;   //!
    TBranch        *b_photon_pfChargedIsoRhoCorr;   //!
    TBranch        *b_TriggerNames;   //!
    TBranch        *b_TriggerPass;   //!
    TBranch        *b_TriggerPrescales;   //!
    TBranch        *b_photon_nonPrompt;   //!

    ReadTree(TTree *tree);

    virtual ~ReadTree();

    int BinNumber();
    
    //member function to identify events where at least one best photon is present ,it returns the position of best photon in the photonCands branch of the tree 
    int photonIndex();

    //baseline selection function
    bool PassBaseLine();

    bool isBarrelPhoton();
    bool isEndcapPhoton();

    bool PASStrigger();

    //As different condtions are required for different Samples so three different fuctions
    bool PASS_PhotonCRForGJets();
    bool PASS_PhotonCRForQCD();
    bool PASS_PhotonCRForData();



};

    ReadTree::~ReadTree()

{

   if (!fChain) return;

   delete fChain->GetCurrentFile();

}

   ReadTree::ReadTree(TTree *tree)

{


   fChain=tree;
   photonCands = 0;
   bestPhoton = 0;
   photon_sigmaIetaIeta = 0;
   photon_pfChargedIsoRhoCorr = 0;
   TriggerNames = 0;
   TriggerPass = 0;
   TriggerPrescales = 0;
   photon_nonPrompt = 0;

   fChain->SetBranchAddress("RunNum", &RunNum, &b_RunNum);
   fChain->SetBranchAddress("LumiBlockNum", &LumiBlockNum, &b_LumiBlockNum);
   fChain->SetBranchAddress("EvtNum", &EvtNum, &b_EvtNum);

   fChain->SetBranchAddress("NVtx", &NVtx, &b_NVtx);
   fChain->SetBranchAddress("eeBadScFilter",&eeBadScFilter,&b_eeBadScFilter);
   fChain->SetBranchAddress("eeBadSc4Filter",&eeBadSc4Filter,&b_eeBadSc4Filter);   
   fChain->SetBranchAddress("HBHENoiseFilter",&HBHENoiseFilter,&b_HBHENoiseFilter);
   fChain->SetBranchAddress("HBHEIsoNoiseFilter",&HBHEIsoNoiseFilter,&b_HBHEIsoNoiseFilter);


   fChain->SetBranchAddress("HT", &HT, &b_HT);
   fChain->SetBranchAddress("MHT", &MHT, &b_MHT);
   fChain->SetBranchAddress("NJets", &NJets, &b_NJets);
   fChain->SetBranchAddress("BTags", &BTags, &b_BTags);
   fChain->SetBranchAddress("DeltaPhi1clean", &DeltaPhi1clean, &b_DeltaPhi1clean);
   fChain->SetBranchAddress("DeltaPhi2clean", &DeltaPhi2clean, &b_DeltaPhi2clean);
   fChain->SetBranchAddress("DeltaPhi3clean", &DeltaPhi3clean, &b_DeltaPhi3clean);
   fChain->SetBranchAddress("Weight", &Weight, &b_Weight);
   fChain->SetBranchAddress("puWeight", &puWeight, &b_puWeight);
   fChain->SetBranchAddress("photonCands", &photonCands, &b_photonCands);
   fChain->SetBranchAddress("bestPhoton", &bestPhoton, &b_bestPhoton);
   fChain->SetBranchAddress("photon_nonPrompt", &photon_nonPrompt, &b_photon_nonPrompt);
   fChain->SetBranchAddress("photon_sigmaIetaIeta", &photon_sigmaIetaIeta, &b_photon_sigmaIetaIeta);
   fChain->SetBranchAddress("photon_pfChargedIsoRhoCorr", &photon_pfChargedIsoRhoCorr, &b_photon_pfChargedIsoRhoCorr);
   fChain->SetBranchAddress("TriggerNames", &TriggerNames, &b_TriggerNames);
   fChain->SetBranchAddress("TriggerPass", &TriggerPass, &b_TriggerPass);
   fChain->SetBranchAddress("TriggerPrescales", &TriggerPrescales, &b_TriggerPrescales);


}

    bool ReadTree::PassBaseLine(){

    bool pass=false;
    int Bin_Number= BinNumber();

    if(Bin_Number > -1 ){//bin number > -1 ensures baseline cut is applied 
       pass=true;
      }

    return pass;


    }






    
int ReadTree::photonIndex(){//photon index


    TLorentzVector BestPhoton;
    BestPhoton=bestPhoton->at(0);
    int index=-1;
    for(int iPh=0;iPh<photonCands->size();iPh++){
       TLorentzVector iPhoton=photonCands->at(iPh);
      
       double DeltaR=BestPhoton.DeltaR(iPhoton);       
               if(BestPhoton==iPhoton){
                  index=iPh;          

       }
       }
      

    return index;

}//photon index

    


       //member function to compute the bin numbers
    int ReadTree::BinNumber(){
    
    if( MHT < 200 || HT < 800 || NJets < 7 ) return -1 ;

    int binMHT = -1;
    int binBJet = -1;

    if( MHT > 200 && MHT < 500 ){

      //low MHT
      binMHT = 1;

    }else if( MHT > 500 && MHT < 750 ){

    //medium MHT
      binMHT = 2 ; 

    }else if( MHT > 750 ){
     //high MHT
      binMHT = 3 ; 

    }else
 
       return -1;


    if( BTags >= 0 && BTags <= 1 ){
      //low BJet
      binBJet = 0 ;

      }else if( BTags >=2  ){
      ////high BJet
      binBJet = 1 ;

      }else

        return -1;

    return binBJet*3+binMHT ;



    }
    

    bool ReadTree::isBarrelPhoton(){

     bool Barrel=false;
    if(fabs(bestPhoton->at(0).Eta())< 1.4442){//barrel
            Barrel=true;
           }//barrel

     return Barrel;
     }



     bool ReadTree::isEndcapPhoton(){

     bool Endcap=false;
         if(fabs(bestPhoton->at(0).Eta())> 1.566 && fabs(bestPhoton->at(0).Eta())< 2.5){//endcap
          Endcap=true;
 
          }//endcap

     return Endcap;
     }


    bool ReadTree::PASStrigger(){
               bool Passtrigger=false;
               for(int itr=0;itr<TriggerNames->size();itr++){
             if(TriggerNames->at(itr)=="HLT_Photon90_CaloIdL_PFHT500_v3" && TriggerPass->at(itr)==1){
                Passtrigger=true;
               }
              }
         
       return Passtrigger;
          }


    bool ReadTree::PASS_PhotonCRForGJets(){
       bool PassCR=false;
       int binNumber_GJets = BinNumber();
       int index= photonIndex();
       if(binNumber_GJets >-1 &&  index !=-1 && photon_nonPrompt->at(index) !=1){
             
          PassCR=true;
          }
       return PassCR;

       }


       bool ReadTree::PASS_PhotonCRForQCD(){
       bool PassCR=false;
       int binNumber_QCD = BinNumber();
       int index= photonIndex();
       if(binNumber_QCD >-1 &&  index !=-1 && photon_nonPrompt->at(index) ==1){

          PassCR=true;
          }
       return PassCR;

       }




      bool ReadTree::PASS_PhotonCRForData(){
       bool PassCR=false;
       int binNumber_Data = BinNumber();
       int index= photonIndex();
       if(binNumber_Data >-1 &&  index !=-1 && PASStrigger()==true){

          PassCR=true;
          }
       return PassCR;

       }







