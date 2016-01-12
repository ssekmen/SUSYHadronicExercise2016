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
    double         DeltaPhi1;
    double         DeltaPhi2;
    double         DeltaPhi3;
    double         DeltaPhi4;
    double         Weight;
    double        puWeight;
    vector<double>  *photon_sigmaIetaIeta;
    vector<double>  *photon_pfChargedIsoRhoCorr;
    vector<bool>    *photon_nonPrompt;
    vector<TLorentzVector> *photonCands;
    vector<TLorentzVector> *bestPhoton;
    vector<string>  *TriggerNames;
    vector<int>    *TriggerPass;
    vector<int>     *TriggerPrescales;
    vector<TLorentzVector> *ZCandidates;


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
    TBranch        *b_DeltaPhi1;   //!
    TBranch        *b_DeltaPhi2;   //!
    TBranch        *b_DeltaPhi3;   //!
    TBranch        *b_DeltaPhi4;   //!
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
    TBranch        *b_ZCandidates;   //!


    ReadTree(TTree *tree);

    virtual ~ReadTree();

    int BinNumber();
    
    //member function to identify events where at least one best photon is present ,it returns the position of best photon in the photonCands branch of the tree 
    int photonIndex();

    //baseline selection function
    bool BaselineLoose();

    bool PassIDISo();
 
    bool isBarrelPhoton();
    bool isEndcapPhoton();
    
    bool PASStriggerGJ();
    bool PASStriggerDYm();



    //As different condtions are required for different Samples so three different fuctions
    bool PASS_PhotonCR_GJetsMC();
    bool PASS_PhotonCR_GJetsData();
    bool PASS_PhotonCR_QCDMC();
    bool PASS_DoubleMuCR_DYmMC();
    bool PASS_DoubleMuCR_DYmData();  
    bool PASS_DoubleMuCR_TTJetsMC();
   

    bool IchSR();//charged Isolation signal region
    bool IchSB(); //charged isolation side band

    bool Prompt();
    bool nonPrompt();
  






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
   ZCandidates = 0;


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
   fChain->SetBranchAddress("DeltaPhi1", &DeltaPhi1, &b_DeltaPhi1);
   fChain->SetBranchAddress("DeltaPhi2", &DeltaPhi2, &b_DeltaPhi2);
   fChain->SetBranchAddress("DeltaPhi3", &DeltaPhi3, &b_DeltaPhi3);
   fChain->SetBranchAddress("DeltaPhi4", &DeltaPhi4, &b_DeltaPhi4);
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
   fChain->SetBranchAddress("ZCandidates", &ZCandidates, &b_ZCandidates);

}
    
    bool ReadTree::BaselineLoose(){

    bool pass=false;

    if(HT>500. && MHT>200 && NJets>=4 && DeltaPhi1>0.5 && DeltaPhi2> 0.5 && DeltaPhi3> 0.3 && DeltaPhi4>0.3 ){//bin 
       pass=true;
      }

    return pass;


    }





    
int ReadTree::photonIndex(){//photon index


    TLorentzVector BestPhoton;
    BestPhoton=bestPhoton->at(0);
    int index=-1;
    for(unsigned int iPh=0;iPh<photonCands->size();iPh++){
       TLorentzVector iPhoton=photonCands->at(iPh);
      
               if(BestPhoton==iPhoton  ){
                  index=iPh;          

       }
       }
      

    return index;

}//photon index

    

    bool ReadTree::PassIDISo(){
     bool Pass=false;

     int Phindex=photonIndex();

     if(Phindex > -1 && isBarrelPhoton()==true && photon_sigmaIetaIeta->at(Phindex) < 0.0107 && photon_pfChargedIsoRhoCorr->at(Phindex) <2.67   ){

         Pass =true;
        }else if(Phindex > -1 && isEndcapPhoton()==true && photon_sigmaIetaIeta->at(Phindex) < 0.0272 && photon_pfChargedIsoRhoCorr->at(Phindex) <1.79){
       
        Pass = true;

          }else{
          
        Pass = false;
         
          }
    

     return Pass;


     }






       
    int ReadTree::BinNumber(){
    
    if( MHT < 200 || HT < 500 || NJets < 7 || BTags <2 || DeltaPhi1<0.5 || DeltaPhi2< 0.5 || DeltaPhi3 < 0.3 || DeltaPhi4 <0.3  ) return -1 ;

    int binNum = -1;
    

    if( NJets>=7 && NJets <=8 && MHT > 200 && MHT < 500 && BTags==2 ){

      
      binNum = 1;

    }else if( NJets>=7 && NJets <=8 && MHT > 200 && MHT < 500 && BTags>=3 ){

    
      binNum = 2 ; 

    }else if( NJets>=7 && NJets <=8 && MHT > 500 && BTags>=2 ){
      
      binNum = 3 ; 

    }else if( NJets >=9 && MHT > 200 && MHT < 500 && BTags==2 ){

      binNum = 4;
      
    }else if( NJets>=9  && MHT > 200 && MHT < 500 && BTags>=3 ){
      
      binNum = 5 ;

    }else if( NJets>=9 && MHT > 500 && BTags>=2 ){
      
      binNum = 6 ;

    }else{
     
        return -1;
    }

    return binNum;
               

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
   
   

    bool ReadTree::PASStriggerGJ(){
               bool Passtrigger=false;
               for(unsigned int itr=0;itr<TriggerNames->size();itr++){
             if(TriggerNames->at(itr)=="HLT_Photon90_CaloIdL_PFHT500_v3" && TriggerPass->at(itr)==1){
                Passtrigger=true;
               }
              }
         
       return Passtrigger;
          }


    bool ReadTree::PASStriggerDYm(){
               bool Passtrigger=false;
               for(unsigned int itr=0;itr<TriggerNames->size();itr++){
             if(TriggerNames->at(itr)=="HLT_Mu15_IsoVVVL_PFHT350_v2" && TriggerPass->at(itr)==1){
                Passtrigger=true;
               }
              }

       return Passtrigger;
          }








    bool ReadTree::PASS_PhotonCR_GJetsMC(){
       bool PassCR=false;
       int index= photonIndex();
       if(  index > -1 && photon_nonPrompt->at(index) !=1 && PassIDISo()==true ){
             
          PassCR=true;
          }
       return PassCR;

       }


       bool ReadTree::PASS_PhotonCR_QCDMC(){
       bool PassCR=false;
       int index= photonIndex();
       if(  index > -1 && photon_nonPrompt->at(index) ==1 && PassIDISo()==true){

          PassCR=true;
          }
       return PassCR;

       }




      bool ReadTree::PASS_PhotonCR_GJetsData(){
       bool PassCR=false;
       int index= photonIndex();
       if(  index >-1  && PASStriggerGJ()==true && PassIDISo()==true && NVtx >0 && eeBadScFilter==1 && eeBadSc4Filter==true && HBHENoiseFilter==true && HBHEIsoNoiseFilter==true){

          PassCR=true;
          }
       return PassCR;

       }



    bool ReadTree::PASS_DoubleMuCR_DYmMC(){
    bool PassCR=false;
   
    int numZ=ZCandidates->size();
    double Zmass= ZCandidates->at(0).M();

    if( Zmass > 70. && Zmass < 110 && numZ ==1){

      PassCR=true;


        }

     return PassCR;

    }



    bool ReadTree::PASS_DoubleMuCR_DYmData(){
    bool PassCR=false;


    int numZ=ZCandidates->size();
    double Zmass= ZCandidates->at(0).M();

    if( Zmass > 70. && Zmass < 110 && PASStriggerDYm()==true && numZ==1 && NVtx >0 && eeBadScFilter==1 && eeBadSc4Filter==true && HBHENoiseFilter==true && HBHEIsoNoiseFilter==true){

      PassCR=true;


        }

     return PassCR;

    }


    bool ReadTree::PASS_DoubleMuCR_TTJetsMC(){
    bool PassCR=false;

    int numZ=ZCandidates->size();

    double Zmass= ZCandidates->at(0).M();

    if( Zmass > 70. && Zmass < 110 && numZ==1){

      PassCR=true;


        }

     return PassCR;

    }
   

    bool ReadTree::IchSR(){
     bool Pass=false;
     int index= photonIndex();
    if(isBarrelPhoton()==true && index  > -1 && photon_pfChargedIsoRhoCorr->at(index) <2.67 ){
       Pass=true;

        }else if(isEndcapPhoton()==true && index  > -1 && photon_pfChargedIsoRhoCorr->at(index) <1.79){
      Pass=true;

        }else{

      Pass=false;

         }
      
      return Pass;
    }


    bool ReadTree::IchSB(){
     bool Pass=false;
     int index= photonIndex();
    if(isBarrelPhoton()==true && index  > -1 && photon_pfChargedIsoRhoCorr->at(index) > 2.67 ){
       Pass=true;

        }else if(isEndcapPhoton()==true && index  > -1 && photon_pfChargedIsoRhoCorr->at(index) > 1.79){
      Pass=true;

        }else{

      Pass=false;

         }

      return Pass;
    }



   bool ReadTree::Prompt(){
     bool Pass=false;
     int index= photonIndex();
    if( index  > -1 && photon_nonPrompt->at(index) != true ){
       Pass=true;

         }

      return Pass;
    }



    bool ReadTree::nonPrompt(){
     bool Pass=false;
     int index= photonIndex();
    if( index  > -1 && photon_nonPrompt->at(index) == true ){
       Pass=true;

         }

      return Pass;
    }





