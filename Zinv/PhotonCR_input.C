#include<vector>
#include "TH1.h"
#include "TH2.h"
#include "TGraph.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TString.h"
#include "TDirectory.h"
#include "ReadTree.cc"
#include "SetStyle.C"
#include "StackPlot.C"
#include "ComputeBins.C"
#include <stdio.h>
void PhotonCR_input(){


     TStyle *gStyle = new TStyle("gStyle","Style for P-TDR");
     SetStyle st;
     st.SetPars(gStyle);

     TFile *f =new TFile("PhotonCR_input.root","RECREATE"); 

     TString InputFilePath="/eos/uscms/store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/Run2ProductionV5/";

     TChain* tZinv = new TChain("tree");
     tZinv->Add(InputFilePath+"tree_signal/tree_ZJetsToNuNu_HT*.root");
     ReadTree Zinv(tZinv);

     TChain* tGJets = new TChain("tree");
     tGJets->Add(InputFilePath+"tree_GJet_CleanVarsGJ/tree_GJets_HT-*.root");
     ReadTree GJets(tGJets);

     TChain* tQCD = new TChain("tree");
     tQCD->Add(InputFilePath+"tree_GJet_CleanVarsGJ/tree_QCD_HT*.root");
     ReadTree QCD(tQCD);

     TChain* tData = new TChain("tree");
     tData->Add(InputFilePath+"tree_GJet_CleanVarsGJ/tree_SinglePhoton_*.root");
     ReadTree Data(tData);


     double Lumi=2110.;

     int nBinsHT=13;
     int nBinsMHT=7;
     int nBinsNJ=6;
     double HTbins[14]={500.,600.,700,800,900,1000.,1100,1200.,1300,1400,1500,1700,2000,3000};
     double MHTbins[8]={200.,300,400,500.,600,750.,1000,1500.};
     double NJetsbins[7]={4,5,6,7,8,9,12};

     int nSearchBins=6;
     double BinMax=6.5;

     TH1F *hHT_GJets=new TH1F("hHT_GJets","hHT_GJets",nBinsHT,HTbins);
     TH1F *hMHT_GJets=new TH1F("hMHT_GJets","hMHT_GJets",nBinsMHT,MHTbins);
     TH1F *hNJets_GJets=new TH1F("hNJets_GJets","hNJets_GJets",nBinsNJ,NJetsbins);

     TH1F *hHT_QCD=new TH1F("hHT_QCD","hHT_QCD",nBinsHT,HTbins);
     TH1F *hMHT_QCD=new TH1F("hMHT_QCD","hMHT_QCD",nBinsMHT,MHTbins);
     TH1F *hNJets_QCD=new TH1F("hNJets_QCD","hNJets_QCD",nBinsNJ,NJetsbins);

     TH1F *hHT_Data=new TH1F("hHT_Data","hHT_Data",nBinsHT,HTbins);
     TH1F *hMHT_Data=new TH1F("hMHT_Data","hMHT_Data",nBinsMHT,MHTbins);
     TH1F *hNJets_Data=new TH1F("hNJets_Data","hNJets_Data",nBinsNJ,NJetsbins);

     TH1F  *hSearchBins_Zinv=new TH1F("hSearchBins_Zinv","hSearchBins_Zinv",nSearchBins,0.5,BinMax);
     TH1F  *hSearchBins_GJets=new TH1F("hSearchBins_GJets","hSearchBins_GJets",nSearchBins,0.5,BinMax);

     TH1F  *hSearchBins_GJets_EB=new TH1F("hSearchBins_GJets_EB","hSearchBins_GJets_EB",nSearchBins,0.5,BinMax);
     TH1F  *hSearchBins_GJets_EE=new TH1F("hSearchBins_GJets_EE","hSearchBins_GJets_EB",nSearchBins,0.5,BinMax);

     TH1F  *hSearchBins_QCD_EB=new TH1F("hSearchBins_QCD_EB","hSearchBins_QCD_EB",nSearchBins,0.5,BinMax);
     TH1F  *hSearchBins_QCD_EE=new TH1F("hSearchBins_QCD_EE","hSearchBins_QCD_EB",nSearchBins,0.5,BinMax);

     TH1F  *hSearchBins_Data_EB=new TH1F("hSearchBins_Data_EB","hSearchBins_Data_EB",nSearchBins,0.5,BinMax);
     TH1F  *hSearchBins_Data_EE=new TH1F("hSearchBins_Data_EE","hSearchBins_Data_EB",nSearchBins,0.5,BinMax);
     
     int maxEvents_Zinv=tZinv->GetEntries();

     for(int iEv=0;iEv<maxEvents_Zinv;iEv++){//Looping over Zinv MC///////////////////////////////////////
     tZinv->GetEntry(iEv);
     if(iEv % 100000==0){cout<<"Event no Zinv : "<<iEv<<endl;}

     int binNumber_Zinv = computeBin( Zinv.MHT, Zinv.HT, Zinv.NJets, Zinv.BTags, k13TeV);

     double weight=0;
     if(binNumber_Zinv >-1 /* && Zinv.BTags==0*/){//Filling Yield(N_Obsereved) in Zinv

          hSearchBins_Zinv->Fill(binNumber_Zinv,(Lumi*Zinv.Weight)/1.23);
         
       }


     }//Lopping over Zinv MC/

     for(int iEv=0;iEv<tGJets->GetEntries();iEv++){//Looping over GJets MC events
     tGJets->GetEntry(iEv);
     if(iEv % 100000==0){cout<<"Event no [GJets] : "<<iEv<<endl;}
     
     int binNumber_GJets = computeBin( GJets.MHT, GJets.HT, GJets.NJets, GJets.BTags, k13TeV);
     int index=GJets.photonIndex();
     if(binNumber_GJets >-1 && /*GJets.BTags==0 &&*/ index !=-1 && GJets.photon_nonPrompt->at(index) !=1){// Baseline + prompt Photon criteria
         hHT_GJets->Fill(GJets.HT,Lumi*GJets.Weight);
         hMHT_GJets->Fill(GJets.MHT,Lumi*GJets.Weight);
         hNJets_GJets->Fill(GJets.NJets,Lumi*GJets.Weight);

         hSearchBins_GJets->Fill(binNumber_GJets,Lumi*GJets.Weight);

         if(fabs(GJets.bestPhoton->at(0).Eta())< 1.4442){//barrel
           hSearchBins_GJets_EB->Fill(binNumber_GJets,Lumi*GJets.Weight);
           }//barrel

         if(fabs(GJets.bestPhoton->at(0).Eta())> 1.566 && fabs(GJets.bestPhoton->at(0).Eta())< 2.5){//endcap
          hSearchBins_GJets_EE->Fill(binNumber_GJets,Lumi*GJets.Weight);
          }//endcap 
      }//Baseline + Prompt Photon criteria

     }//Looping over GJets MC events



    for(int iEv=0;iEv<tQCD->GetEntries();iEv++){//Looping over QCD MC events
     tQCD->GetEntry(iEv);
     if(iEv % 100000==0){cout<<"Event no [QCD] : "<<iEv<<endl;}

     int binNumber_QCD = computeBin( QCD.MHT, QCD.HT, QCD.NJets, QCD.BTags, k13TeV);
     int index=QCD.photonIndex();
     if(binNumber_QCD >-1 && /*QCD.BTags==0 &&*/ index !=-1 && QCD.photon_nonPrompt->at(index) ==1){// Baseline + nonPrompt Photon criteria
         hHT_QCD->Fill(QCD.HT,Lumi*QCD.Weight);
         hMHT_QCD->Fill(QCD.MHT,Lumi*QCD.Weight);
         hNJets_QCD->Fill(QCD.NJets,Lumi*QCD.Weight);

         if(fabs(QCD.bestPhoton->at(0).Eta())< 1.4442){//barrel
           hSearchBins_QCD_EB->Fill(binNumber_QCD,Lumi*QCD.Weight);
           }//barrel

         if(fabs(QCD.bestPhoton->at(0).Eta())> 1.566 && fabs(QCD.bestPhoton->at(0).Eta())< 2.5){//endcap
          hSearchBins_QCD_EE->Fill(binNumber_QCD,Lumi*QCD.Weight);
          }//endcap 
      }//Baseline + nonPrompt Photon criteria

     }//Looping over QCD MC events



    for(int iEv=0;iEv<tData->GetEntries();iEv++){//Looping over Data  events
     tData->GetEntry(iEv);
     if(iEv % 100000==0){cout<<"Event no [Data] : "<<iEv<<endl;}

     int binNumber_Data = computeBin( Data.MHT, Data.HT, Data.NJets, Data.BTags, k13TeV);
     int index=Data.photonIndex();
     if(binNumber_Data >-1 && /*Data.BTags==0 &&*/ index !=-1 ){//

      bool PassTrigger=false;

         for(int itr=0;itr<Data.TriggerNames->size();itr++){
             if(Data.TriggerNames->at(itr)=="HLT_Photon90_CaloIdL_PFHT500_v3" && Data.TriggerPass->at(itr)==1){
                PassTrigger=true;
               }
              }

         if(PassTrigger==true){//Trigger

         hHT_Data->Fill(Data.HT);
         hMHT_Data->Fill(Data.MHT);
         hNJets_Data->Fill(Data.NJets);

                if(fabs(Data.bestPhoton->at(0).Eta())< 1.4442){//barrel
                   hSearchBins_Data_EB->Fill(binNumber_Data);
                       }//barrel

                 if(fabs(Data.bestPhoton->at(0).Eta())> 1.566 && fabs(Data.bestPhoton->at(0).Eta())< 2.5){//endcap
                    hSearchBins_Data_EE->Fill(binNumber_Data);
                       }//endcap

            }//Trigger

      }//Baseline 
      }//Looping over Data events
 
     f->Write();

     TCanvas *c[5];
     DataMC plot;

     plot.Stack(hHT_Data,hHT_GJets,hHT_QCD,c[0],"","Data","MC","HT");
     plot.Stack(hMHT_Data,hMHT_GJets,hMHT_QCD,c[1],"","Data","MC","MHT");
     plot.Stack(hNJets_Data,hNJets_GJets,hNJets_QCD,c[2],"","Data","MC","Njets");

     plot.Stack(hSearchBins_Data_EB,hSearchBins_GJets_EB,hSearchBins_QCD_EB,c[3],"EB","Data","MC","ithBin-EB");
     plot.Stack(hSearchBins_Data_EE,hSearchBins_GJets_EE,hSearchBins_QCD_EE,c[4],"EE","Data","MC","ithBin-EE");


}
