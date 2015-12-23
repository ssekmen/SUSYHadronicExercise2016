#include "ReadTree.cc"
#include "SetStyle.C"
#include "StackPlot.C"

using namespace std;
void PhotonCR_Input(){

     //Sets the style of Canvas ,it is like the tdrStyle function
     TStyle *gStyle = new TStyle("gStyle","Style for P-TDR");
     SetStyle st;
     st.SetPars(gStyle);
     
     //Creates a root file that will store all the histograms needed for prediction later
     TFile *f =new TFile("PhotonCR_Input.root","RECREATE"); 
     //Location of Ntuples
     TString InputFilePath="/eos/uscms/store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/Run2ProductionV5/";
     
     //Reads Zinv MC Ntuples
     TChain* tZinv = new TChain("tree");
     tZinv->Add(InputFilePath+"tree_signal/tree_ZJetsToNuNu_HT*.root");
     ReadTree Zinv(tZinv);

     //Reads GJets MC Ntuples
     TChain* tGJets = new TChain("tree");
     tGJets->Add(InputFilePath+"tree_GJet_CleanVarsGJ/tree_GJets_HT-*.root");
     ReadTree GJets(tGJets);

     //reads QCD MC Ntuples
     TChain* tQCD = new TChain("tree");
     tQCD->Add(InputFilePath+"tree_GJet_CleanVarsGJ/tree_QCD_HT*.root");
     ReadTree QCD(tQCD);

     //Reads Data Ntuples
     TChain* tData = new TChain("tree");
     tData->Add(InputFilePath+"tree_GJet_CleanVarsGJ/tree_SinglePhoton_*.root");
     ReadTree Data(tData);

     //defines integrated luminocity for the Data 
     double Lumi=2110.;


     //define the bin range and sizes for Control Region(CR) plots
     double HTbins[14]={500.,600.,700,800,900,1000.,1100,1200.,1300,1400,1500,1700,2000,3000};
     double MHTbins[8]={200.,300,400,500.,600,750.,1000,1500.};
     double NJetsbins[7]={4,5,6,7,8,9,12};

     //nothing to change here ..this line just numbers of bins in the CR plot
     int nBinsHT=sizeof(HTbins)/sizeof(HTbins[0]) -1;
     int nBinsMHT=sizeof(MHTbins)/sizeof(MHTbins[0]) -1;
     int nBinsNJ=sizeof(NJetsbins)/sizeof(NJetsbins[0]) -1;



     //number of search bins in your analysis
     int nSearchBins=6;
     double BinMax=nSearchBins+0.5;//bin max noh=thing to change here 

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

     for(int iEv=0;iEv<maxEvents_Zinv;iEv++){//Looping over Zinv MC
     tZinv->GetEntry(iEv);
     if(iEv % 100000==0){cout<<"Event no Zinv : "<<iEv<<endl;}

        bool PassBaseline = false;
        PassBaseline=Zinv.PassBaseLine();

     
              if(PassBaseline == true){
              hSearchBins_Zinv->Fill(Zinv.BinNumber(),(Lumi*Zinv.Weight)/1.23);
                                  }


                                             }//Lopping over Zinv MC
     

     
     for(int iEv=0;iEv<tGJets->GetEntries();iEv++){//Looping over GJets MC events
     tGJets->GetEntry(iEv);
     if(iEv % 100000==0){cout<<"Event no [GJets] : "<<iEv<<endl;}
     
     if(GJets.PASS_PhotonCRForGJets()==true){// Pass Photon Control Region Criteria
         hHT_GJets->Fill(GJets.HT,Lumi*GJets.Weight);
         hMHT_GJets->Fill(GJets.MHT,Lumi*GJets.Weight);
         hNJets_GJets->Fill(GJets.NJets,Lumi*GJets.Weight);

         hSearchBins_GJets->Fill(GJets.BinNumber() ,Lumi*GJets.Weight);

         if(GJets.isBarrelPhoton()==true){//barrel
           hSearchBins_GJets_EB->Fill(GJets.BinNumber() ,Lumi*GJets.Weight);
           }//barrel

         if(GJets.isEndcapPhoton()==true){//endcap
          hSearchBins_GJets_EE->Fill(GJets.BinNumber(),Lumi*GJets.Weight);
          }//endcap 
      }//pass Photon Control region Criteria

     }//Looping over GJets MC events


    
    for(int iEv=0;iEv<tQCD->GetEntries();iEv++){//Looping over QCD MC events
     tQCD->GetEntry(iEv);
     if(iEv % 100000==0){cout<<"Event no [QCD] : "<<iEv<<endl;}

     if(QCD.PASS_PhotonCRForQCD()==true){// Photon Control Region Criteria for QCD
         hHT_QCD->Fill(QCD.HT,Lumi*QCD.Weight);
         hMHT_QCD->Fill(QCD.MHT,Lumi*QCD.Weight);
         hNJets_QCD->Fill(QCD.NJets,Lumi*QCD.Weight);

         if(QCD.isBarrelPhoton()==true){//barrel
           hSearchBins_QCD_EB->Fill(QCD.BinNumber(),Lumi*QCD.Weight);
           }//barrel

         if(QCD.isEndcapPhoton()==true){//endcap
          hSearchBins_QCD_EE->Fill(QCD.BinNumber(),Lumi*QCD.Weight);
          }//endcap 
      }//Photon Control Region Criteria for QCD

     }//Looping over QCD MC events



    for(int iEv=0;iEv<tData->GetEntries();iEv++){//Looping over Data  events
     tData->GetEntry(iEv);
     if(iEv % 100000==0){cout<<"Event no [Data] : "<<iEv<<endl;}

     if(Data.PASS_PhotonCRForData()==true ){//Pass Photon CR criteria for Data


         hHT_Data->Fill(Data.HT);
         hMHT_Data->Fill(Data.MHT);
         hNJets_Data->Fill(Data.NJets);

                if(Data.isBarrelPhoton()==true){//barrel
                   hSearchBins_Data_EB->Fill(Data.BinNumber());
                       }//barrel

                 if(Data.isEndcapPhoton()==true){//endcap
                    hSearchBins_Data_EE->Fill(Data.BinNumber());
                       }//endcap


      }//Pass Photon CR criteria for Data
      }//Looping over Data events
 
     f->Write();

     TCanvas *c[5];//initializes 5 canvases .you can create more if you decide to somesomething else
     DataMC plot;//A class for plotting stacked MC vs data ,the cuts and legends are set as Photon CR cuts,look Stack.C 

     //HT,MHT,NJets distributions
     plot.Stack(hHT_Data,hHT_GJets,hHT_QCD,c[0],"","Data","MC","HT");
     plot.Stack(hMHT_Data,hMHT_GJets,hMHT_QCD,c[1],"","Data","MC","MHT");
     plot.Stack(hNJets_Data,hNJets_GJets,hNJets_QCD,c[2],"","Data","MC","Njets");

     //distributions of number of observed(Data and MC ) photons for barrel and endcap 
     plot.Stack(hSearchBins_Data_EB,hSearchBins_GJets_EB,hSearchBins_QCD_EB,c[3],"EB","Data","MC","ithBin-EB");
     plot.Stack(hSearchBins_Data_EE,hSearchBins_GJets_EE,hSearchBins_QCD_EE,c[4],"EE","Data","MC","ithBin-EE");
     

}
