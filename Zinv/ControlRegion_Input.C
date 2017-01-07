#include "TH1F.h"
#include "ReadTree.cc"
#include "SetStyle.C"
#include "StackPlot.C"
using namespace std;
void ControlRegion_Input(){

     //Sets the style of Canvas ,it is like the tdrStyle function
     TStyle *gStyle = new TStyle("gStyle","Style for P-TDR");
     SetStyle st;
     st.SetPars(gStyle);
    
     
     //Creates a root file that will store all the histograms needed for prediction later
     TFile *f =new TFile("ControlRegion_Input.root","RECREATE"); 
     //Location of Ntuples
     //TString InputFilePath="root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/Run2ProductionV5/";
     TString InputFilePath="root://cmseos.fnal.gov//store/user/cmsdas/2017/long_exercises/SUSYHadronic/";
     
     //Reads Zinv MC Ntuples
     TChain* tZinv = new TChain("tree");
     tZinv->Add(InputFilePath+"tree_signal/tree_ZJetsToNuNu_HT*.root");
     ReadTree Zinv(tZinv);

     //Reads GJets MC Ntuples
     TChain* tGJets = new TChain("tree");
     tGJets->Add(InputFilePath+"tree_GJet_CleanVarsGJloose/tree_GJets_HT-*.root");
     ReadTree GJets(tGJets);

     //reads QCD MC Ntuples
     TChain* tQCD = new TChain("tree");
     tQCD->Add(InputFilePath+"tree_GJet_CleanVarsGJloose/tree_QCD_HT*.root");
     ReadTree QCD(tQCD);

     TChain* tDYm = new TChain("tree");
     tDYm->Add(InputFilePath+"tree_DYm_CleanVarsDY/tree_DYJetsToLL_M-50_HT-*.root");
     ReadTree DYm(tDYm);

     TChain* tTTJ = new TChain("tree");
     tTTJ->Add(InputFilePath+"tree_DYm_CleanVarsDY/tree_TTJets_HT-*.root");
     ReadTree TTJ(tTTJ);

     //Reads Data Ntuples
     TChain* tDataGJ = new TChain("tree");
     tDataGJ->Add(InputFilePath+"tree_GJet_CleanVarsGJloose/tree_SinglePhoton_*.root");
     ReadTree DataGJ(tDataGJ);


     TChain* tDataDYm = new TChain("tree");
     tDataDYm->Add(InputFilePath+"tree_DYm_CleanVarsDY/tree_SingleMuon_*.root");
     ReadTree DataDYm(tDataDYm);


     //defines integrated luminocity for the Data 
     double Lumi=2262.;


     //define the bin range and sizes for Control Region(CR) plots
     double HTbins[14]={500.,600.,700,800,900,1000.,1100,1200.,1300,1400,1500,1700,2000,3000};
     double MHTbins[8]={200.,300,400,500.,600,750.,1000,1500.};
     double NJetsbins[7]={4,5,6,7,8,9,12};
     double BJetsbins[7]={0,1,2,3,4,5,6};
     //nothing to change here ..this line just numbers of bins in the CR plot
     int nBinsHT=sizeof(HTbins)/sizeof(HTbins[0]) -1;
     int nBinsMHT=sizeof(MHTbins)/sizeof(MHTbins[0]) -1;
     int nBinsNJ=sizeof(NJetsbins)/sizeof(NJetsbins[0]) -1;
     int nBinsBJ=sizeof(BJetsbins)/sizeof(BJetsbins[0]) -1;


     //number of search bins in your analysis
     int nSearchBins=6;
     double BinMax=nSearchBins+0.5;//bin max noh=thing to change here 

     TH1F *hHT_GJets=new TH1F("hHT_GJets","hHT_GJets",nBinsHT,HTbins);
     TH1F *hMHT_GJets=new TH1F("hMHT_GJets","hMHT_GJets",nBinsMHT,MHTbins);
     TH1F *hNJets_GJets=new TH1F("hNJets_GJets","hNJets_GJets",nBinsNJ,NJetsbins);
     TH1F *hBJets_GJets=new TH1F("hBJets_GJets","hBJets_GJets",nBinsBJ,BJetsbins);


     TH1F *hHT_QCD=new TH1F("hHT_QCD","hHT_QCD",nBinsHT,HTbins);
     TH1F *hMHT_QCD=new TH1F("hMHT_QCD","hMHT_QCD",nBinsMHT,MHTbins);
     TH1F *hNJets_QCD=new TH1F("hNJets_QCD","hNJets_QCD",nBinsNJ,NJetsbins);
     TH1F *hBJets_QCD=new TH1F("hBJets_QCD","hBJets_QCD",nBinsBJ,BJetsbins);


     TH1F *hHT_DYm=new TH1F("hHT_DYm","hHT_DYm",nBinsHT,HTbins);
     TH1F *hMHT_DYm=new TH1F("hMHT_DYm","hMHT_DYm",nBinsMHT,MHTbins);
     TH1F *hNJets_DYm=new TH1F("hNJets_DYm","hNJets_DYm",nBinsNJ,NJetsbins);
  
     TH1F *hHT_TTJ=new TH1F("hHT_TTJ","hHT_TTJ",nBinsHT,HTbins);
     TH1F *hMHT_TTJ=new TH1F("hMHT_TTJ","hMHT_TTJ",nBinsMHT,MHTbins);
     TH1F *hNJets_TTJ=new TH1F("hNJets_TTJ","hNJets_TTJ",nBinsNJ,NJetsbins);   

 
     TH1F *hHT_DataGJ=new TH1F("hHT_DataGJ","hHT_DataGJ",nBinsHT,HTbins);
     TH1F *hMHT_DataGJ=new TH1F("hMHT_DataGJ","hMHT_DataGJ",nBinsMHT,MHTbins);
     TH1F *hNJets_DataGJ=new TH1F("hNJets_DataGJ","hNJets_DataGJ",nBinsNJ,NJetsbins);
     TH1F *hBJets_DataGJ=new TH1F("hBJets_DataGJ","hBJets_DataGJ",nBinsBJ,BJetsbins);


     TH1F *hHT_DataDYm=new TH1F("hHT_DataDYm","hHT_DataDYm",nBinsHT,HTbins);
     TH1F *hMHT_DataDYm=new TH1F("hMHT_DataDYm","hMHT_DataDYm",nBinsMHT,MHTbins);
     TH1F *hNJets_DataDYm=new TH1F("hNJets_DataDYm","hNJets_DataDYm",nBinsNJ,NJetsbins);


     TH1F  *hSearchBins_Zinv=new TH1F("hSearchBins_Zinv","hSearchBins_Zinv",nSearchBins,0.5,BinMax);
     TH1F  *hSearchBins_GJets=new TH1F("hSearchBins_GJets","hSearchBins_GJets",nSearchBins,0.5,BinMax);

     TH1F  *hSearchBins_GJets_EB=new TH1F("hSearchBins_GJets_EB","hSearchBins_GJets_EB",nSearchBins,0.5,BinMax);
     TH1F  *hSearchBins_GJets_EE=new TH1F("hSearchBins_GJets_EE","hSearchBins_GJets_EB",nSearchBins,0.5,BinMax);

     TH1F  *hSearchBins_QCD_EB=new TH1F("hSearchBins_QCD_EB","hSearchBins_QCD_EB",nSearchBins,0.5,BinMax);
     TH1F  *hSearchBins_QCD_EE=new TH1F("hSearchBins_QCD_EE","hSearchBins_QCD_EB",nSearchBins,0.5,BinMax);

     TH1F  *hSearchBins_DataGJ_EB=new TH1F("hSearchBins_DataGJ_EB","hSearchBins_DataGJ_EB",nSearchBins,0.5,BinMax);
     TH1F  *hSearchBins_DataGJ_EE=new TH1F("hSearchBins_DataGJ_EE","hSearchBins_DataGJ_EB",nSearchBins,0.5,BinMax);
    
    



 
     int maxEvents_Zinv=tZinv->GetEntries();
     cout<<"Zinv max events: "<<maxEvents_Zinv<<endl;
     for(int iEv=0;iEv<maxEvents_Zinv;iEv++){//Looping over Zinv MC
     tZinv->GetEntry(iEv);
     if(iEv % 100000==0){cout<<"Event no Zinv : "<<iEv<<endl;}


     
              if(Zinv.BinNumber() > -1){
              hSearchBins_Zinv->Fill(Zinv.BinNumber(),(Lumi*Zinv.Weight)/1.23);
                                  }


                                             }//Lopping over Zinv MC
     

    
     cout<<"max events GJets: "<<tGJets->GetEntries()<<endl; 
     for(int iEv=0;iEv<tGJets->GetEntries();iEv++){//Looping over GJets MC events
     tGJets->GetEntry(iEv);
     if(iEv % 100000==0){cout<<"Event no [GJets] : "<<iEv<<endl;}
     
     if(GJets.PASS_PhotonCR_GJetsMC()==true){// Pass Photon Control Region Criteria

         if(GJets.BaselineLoose()==true){
         hHT_GJets->Fill(GJets.HT,Lumi*GJets.Weight);
         hMHT_GJets->Fill(GJets.MHT,Lumi*GJets.Weight);
         hNJets_GJets->Fill(GJets.NJets,Lumi*GJets.Weight);
         hBJets_GJets->Fill(GJets.BTags,Lumi*GJets.Weight);

            }


          if(GJets.BinNumber() > -1){//
          hSearchBins_GJets->Fill(GJets.BinNumber() ,Lumi*GJets.Weight);

         if(GJets.isBarrelPhoton()==true){//barrel
           hSearchBins_GJets_EB->Fill(GJets.BinNumber() ,Lumi*GJets.Weight);
           }//barrel

         if(GJets.isEndcapPhoton()==true){//endcap
          hSearchBins_GJets_EE->Fill(GJets.BinNumber(),Lumi*GJets.Weight);
          }//endcap
          }//
  
      }//pass Photon Control region Criteria

     }//Looping over GJets MC events


    
    for(int iEv=0;iEv<tQCD->GetEntries();iEv++){//Looping over QCD MC events
     tQCD->GetEntry(iEv);
     if(iEv % 100000==0){cout<<"Event no [QCD] : "<<iEv<<endl;}
     if(QCD.PASS_PhotonCR_QCDMC()==true){// Photon Control Region Criteria for QCD

         if(QCD.BaselineLoose()==true){
         hHT_QCD->Fill(QCD.HT,Lumi*QCD.Weight);
         hMHT_QCD->Fill(QCD.MHT,Lumi*QCD.Weight);
         hNJets_QCD->Fill(QCD.NJets,Lumi*QCD.Weight);
         hBJets_QCD->Fill(QCD.BTags,Lumi*QCD.Weight);

            }


           if(QCD.BinNumber() > -1){//
         if(QCD.isBarrelPhoton()==true){//barrel
           hSearchBins_QCD_EB->Fill(QCD.BinNumber(),Lumi*QCD.Weight);
           }//barrel

         if(QCD.isEndcapPhoton()==true){//endcap
          hSearchBins_QCD_EE->Fill(QCD.BinNumber(),Lumi*QCD.Weight);
          }//endcap 
          }
      }//Photon Control Region Criteria for QCD

     }//Looping over QCD MC events


    
    for(int iEv=0;iEv<tDataGJ->GetEntries();iEv++){//Looping over Data  events
     tDataGJ->GetEntry(iEv);
     if(iEv % 100000==0){cout<<"Event no [Data GJ] : "<<iEv<<endl;}

     



     if(DataGJ.PASS_PhotonCR_GJetsData()==true  ){//Pass Photon CR criteria for Data

         if(DataGJ.BaselineLoose()==true){
         hHT_DataGJ->Fill(DataGJ.HT);
         hMHT_DataGJ->Fill(DataGJ.MHT);
         hNJets_DataGJ->Fill(DataGJ.NJets);
         hBJets_DataGJ->Fill(DataGJ.BTags);

            }
                  if(DataGJ.BinNumber() > -1){//
                if(DataGJ.isBarrelPhoton()==true){//barrel
                   hSearchBins_DataGJ_EB->Fill(DataGJ.BinNumber());
                       }//barrel

                 if(DataGJ.isEndcapPhoton()==true){//endcap
                    hSearchBins_DataGJ_EE->Fill(DataGJ.BinNumber());
                       }//endcap
                      }//

      }//Pass Photon CR criteria for Data
      }//Looping over Data events



    for(int iEv=0;iEv<tDYm->GetEntries();iEv++){//Looping over Z(tomumu)+ Jets MC events
     tDYm->GetEntry(iEv);
     if(iEv % 100000==0){cout<<"Event no [DYm] : "<<iEv<<endl;}

     if(DYm.PASS_DoubleMuCR_DYmMC()==true && DYm.BaselineLoose()==true){// Pass Photon Control Region Criteria
         hHT_DYm->Fill(DYm.HT,Lumi*DYm.Weight);
         hMHT_DYm->Fill(DYm.MHT,Lumi*DYm.Weight);
         hNJets_DYm->Fill(DYm.NJets,Lumi*DYm.Weight);


       }// Pass Photon Control Region Criteria

     }//Looping over Z(tomumu)+Jets MC events



     for(int iEv=0;iEv<tTTJ->GetEntries();iEv++){//Looping over TTJets MC events
     tTTJ->GetEntry(iEv);
     if(iEv % 100000==0){cout<<"Event no [TTJets] : "<<iEv<<endl;}

     if(TTJ.PASS_DoubleMuCR_TTJetsMC()==true && TTJ.BaselineLoose()==true){// Pass Di muon Control Region Criteria
         hHT_TTJ->Fill(TTJ.HT,Lumi*TTJ.Weight);
         hMHT_TTJ->Fill(TTJ.MHT,Lumi*TTJ.Weight);
         hNJets_TTJ->Fill(TTJ.NJets,Lumi*TTJ.Weight);

       }// Pass Dimuon Control Region Criteria

     }//Looping over TTJets MC events




    for(int iEv=0;iEv<tDataDYm->GetEntries();iEv++){//Looping over Di muonData events
     tDataDYm->GetEntry(iEv);
     if(iEv % 100000==0){cout<<"Event no [DataDYm] : "<<iEv<<endl;}




     if(DataDYm.PASS_DoubleMuCR_DYmData()==true && DataDYm.BaselineLoose()==true){// Pass Dimuon Control Region Criteria
         hHT_DataDYm->Fill(DataDYm.HT);
         hMHT_DataDYm->Fill(DataDYm.MHT);
         hNJets_DataDYm->Fill(DataDYm.NJets);         




       }//Pass pho


     }//Looping over Dimuon events

 
     f->Write();

     TCanvas *c[7];//initializes 5 canvases .you can create more if you decide to somesomething else
     DataMC plot;//A class for plotting stacked MC vs data ,the cuts and legends are set as Photon CR cuts,look Stack.C 

     //HT,MHT,NJets distributions
     plot.Stack(hHT_DataGJ,hHT_GJets,hHT_QCD,c[0],"","Data","MC","HT");
     plot.Stack(hMHT_DataGJ,hMHT_GJets,hMHT_QCD,c[1],"","Data","MC","MHT");
     plot.Stack(hNJets_DataGJ,hNJets_GJets,hNJets_QCD,c[2],"","Data","MC","Njets");
     plot.Stack(hBJets_DataGJ,hBJets_GJets,hBJets_QCD,c[3],"","Data","MC","Bjets");


     //distributions of number of observed(Data and MC ) photons for barrel and endcap 
     plot.Stack(hSearchBins_DataGJ_EB,hSearchBins_GJets_EB,hSearchBins_QCD_EB,c[4],"EB","Data","MC","Search_Bin_Number_Barrel");
     plot.Stack(hSearchBins_DataGJ_EE,hSearchBins_GJets_EE,hSearchBins_QCD_EE,c[5],"EE","Data","MC","Search_Bin_Number_Endcap");
     

}
