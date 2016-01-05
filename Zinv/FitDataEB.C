#ifndef __CINT__
#include "RooGlobalFunc.h"
#endif
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooDataHist.h"
#include "RooGaussian.h"
#include "RooPlot.h"
#include "TTree.h"
#include "TH1D.h"
#include "TRandom.h"
#include<iostream>
#include "TCanvas.h"
#include "RooPlot.h"
#include "TAxis.h"
#include<vector>
#include "TH1.h"
#include "TH2.h"
#include "TGraph.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TString.h"
#include "TDirectory.h"
#include "ReadTreePurity.cc"
#include "SetStyle.C"

using namespace std;
using namespace RooFit ;
void FitDataEB(){

   //SR = stands for Signal region, i.e. Charged Isolation < 2.76 (for barrel) 
   // SB = stands for Side band i.e. Charged Isolation > 2.76 (for barrel)  

   //Scut= sigmaIetaIeta cut i.e. SigmaIetaIeta < 0.0107

   double rangeMax=0.02;
   double rangeMin=0.005;
   int nBins=40;

   TH1F *hpromptSR_Scut=new TH1F("hpromptSR_Scut","hpromptSR_Scut",nBins,rangeMin,rangeMax);
   hpromptSR_Scut->SetLineWidth(1);

   TH1F *hpromptPlusnonPromptSR_Scut=new TH1F("hpromptPlusnonPromptSR_Scut","hpromptPlusnonPromptSR_Scut",nBins,rangeMin,rangeMax);
   hpromptPlusnonPromptSR_Scut->SetLineWidth(1);
   

   TH1F *hpromptSR=new TH1F("hpromptSR","hpromptSR",nBins,rangeMin,rangeMax);
   hpromptSR->SetLineWidth(1);
  

   TH1F *hnonpromptSR=new TH1F("hnonpromptSR","hnonpromptSR",nBins,rangeMin,rangeMax);
   hnonpromptSR->SetLineWidth(1);

   TH1F *hnonpromptSB=new TH1F("hnonpromptSB","hnonpromptSB",nBins,rangeMin,rangeMax);
   hnonpromptSB->SetLineWidth(1);

   TH1F *hDataSR=new TH1F("hDataSR","hDataSR",nBins,rangeMin,rangeMax);
   hDataSR->SetLineWidth(1);
   



   TChain* tGJets = new TChain("SimpleTree");
   tGJets->Add("/eos/uscms/store/user/bmahakud/CMSDASLPC2016/GJetsMC.root");
   ReadTree GJets(tGJets);


   TChain* tQCD = new TChain("SimpleTree");
   tQCD->Add("/eos/uscms/store/user/bmahakud/CMSDASLPC2016/QCDMC.root");
   ReadTree QCD(tQCD);


   TChain* tData = new TChain("SimpleTree");
   tData->Add("/eos/uscms/store/user/bmahakud/CMSDASLPC2016/SinglePhoton2p1ifb.root");//Single Photon Data
   ReadTree Data(tData);
   
   
    for(int iEv=0;iEv<tGJets->GetEntries();iEv++){//GJets event loop
    tGJets->GetEntry(iEv);
    if(iEv % 100000==0){cout<<"Event no GJets : "<<iEv<<endl;}

    if(GJets.isBarrel==1 && GJets.nonPrompt !=1 && GJets.BaseLine()==true &&  GJets.IchSR==1 && GJets.BJets==0 ){//prompt 
 
   
          hpromptSR->Fill(GJets.SigmaIetaIeta,GJets.evWeight);
         // if(GJets.SigmaIetaIeta < 0.02){
          //       }

          if(GJets.SigmaIetaIeta < 0.0107){
          hpromptSR_Scut->Fill(GJets.SigmaIetaIeta,GJets.evWeight);
          hpromptPlusnonPromptSR_Scut->Fill(GJets.SigmaIetaIeta,GJets.evWeight);
                 }

   }//prompt


   }//GJets event loop

  
    for(int iEv=0;iEv<tQCD->GetEntries();iEv++){//QCD event loop
    tQCD->GetEntry(iEv);
    if(iEv % 100000==0){cout<<"Event no QCD : "<<iEv<<endl;}

    if(QCD.nonPrompt ==1 && QCD.BaseLine()==true  && QCD.isBarrel==1 && QCD.BJets==0){//non prompt 
   
    if(QCD.IchSR==1 &&  QCD.SigmaIetaIeta < 0.02 ){//2
    hnonpromptSR->Fill(QCD.SigmaIetaIeta,QCD.evWeight);
    
    
      if(QCD.SigmaIetaIeta < 0.0107){//1
      
        hpromptPlusnonPromptSR_Scut->Fill(QCD.SigmaIetaIeta,QCD.evWeight);

       }//1
      }//2
   
    if(QCD.IchSB==1 && QCD.SigmaIetaIeta < 0.02){//3
    hnonpromptSB->Fill(QCD.SigmaIetaIeta,QCD.evWeight);
   
       }//3

   }//non prompt

   }//QCD event loop



   

   
    for(int iEv=0;iEv<tData->GetEntries();iEv++){//Data
    tData->GetEntry(iEv);
    if(iEv % 1000000==0){cout<<"Event no Data : "<<iEv<<endl;}

      bool PassTrigger=false;
          
         for(int itr=0;itr<Data.TriggerNames->size();itr++){
             
             if(Data.TriggerNames->at(itr)=="HLT_Photon90_CaloIdL_PFHT500_v3" && Data.TriggerPass->at(itr)==1){
                PassTrigger=true;
                     }
                 }

           ///You can set Data.NJets >=7 and see how purity changes

          if( PassTrigger==true && Data.isBarrel==1 && Data.BaseLine()==true  && Data.IchSR==1 && Data.BJets==0){//photon SR


           if(Data.SigmaIetaIeta < 0.02){
             hDataSR->Fill(Data.SigmaIetaIeta);  
                  }



        }//Photon SR

       }//Data


    
    RooRealVar SIeta("SIeta","#sigma_{i#etai#eta}",rangeMin,rangeMax);
    RooDataHist dataSR("dataSR","dataSR",SIeta,Import(*hDataSR));

    RooDataHist MCprompt("MCprompt","MCprompt",SIeta,Import(*hpromptSR));
    RooDataHist MCnonPrompt("MCnonPrompt","MCnonPrompt",SIeta,Import(*hnonpromptSB));


    RooHistPdf PromptPDF("PromptPDF","PromptPDF",SIeta,MCprompt,0);
    RooHistPdf nonPromptPDF("nonPromptPDF","nonPromptPDF",SIeta,MCnonPrompt,0);


    RooRealVar Fitfrac("Fitfrac","Fitfrac",0.7,0.,1.5);

    RooAddPdf PDF("PDF","PSR-NPSB-PDF", RooArgList(PromptPDF,nonPromptPDF),Fitfrac);

  

    TPaveText *tpav_txt = new TPaveText(0.43043478,0.6548342,0.7652174,0.8510471,"brNDC");

    tpav_txt->SetBorderSize(0);
    tpav_txt->SetFillStyle(0);
    tpav_txt->SetTextAlign(11);
    tpav_txt->SetTextFont(42);
    tpav_txt->SetTextSize(0.03);
    double pureMCPurity=hpromptSR_Scut->Integral()/hpromptPlusnonPromptSR_Scut->Integral();
    

    char mcPurity[100];
    sprintf(mcPurity,"Purity(MC, EB): %4.3f ",pureMCPurity); 
    tpav_txt->AddText(mcPurity);

    
    RooPlot* frame4 = SIeta.frame(Title("CMS #it{Preliminary}                        2.1 fb^{-1}, 13 TeV"));

    dataSR.plotOn(frame4,Name("dataSR1"));  
    PDF.fitTo(dataSR);
    PDF.plotOn(frame4);
    

  
    PDF.plotOn(frame4,Name("PromptPDF"),Components(RooArgList(PromptPDF)), LineColor(kRed),LineStyle(2), LineWidth(2));
    PDF.plotOn(frame4,Name("nonPromptPDF"),Components(RooArgList(nonPromptPDF)), LineColor(kGreen),LineStyle(2), LineWidth(3));
    PDF.plotOn(frame4,Name("PDF"),Components(RooArgList(PDF)), LineColor(kBlue),LineStyle(1), LineWidth(2));
  


    TLegend *leg=new TLegend(00.5402358,0.4731495,0.7441038,0.6734398,NULL,"brNDC");
    leg->SetTextFont(62);
    leg->SetLineColor(1);
    leg->SetLineStyle(1);
    leg->SetLineWidth(3);
    leg->SetFillColor(0);
    leg->SetFillStyle(1001);
    leg->SetShadowColor(0);
    leg->SetDrawOption(0);
    leg->SetBorderSize(0);
    leg->SetTextSize(0.02);

  
    leg->AddEntry(frame4->findObject("PromptPDF"),"Prompt PDF(from MC),EB","l");
    leg->AddEntry(frame4->findObject("nonPromptPDF"),"NonPrompt PDF(from MC),EB","l");
    leg->AddEntry(frame4->findObject("PDF"),"P+NP PDF(from MC),EB","l");
    leg->AddEntry(frame4->findObject("dataSR1"),"Data,EB","P");

  
  
  


    SIeta.setRange("SR",rangeMin,0.0107);
    RooAbsReal* fracP = PromptPDF.createIntegral(SIeta,NormSet(SIeta),Range("SR"));
    RooAbsReal* Ip=PromptPDF.createIntegral(SIeta);
    RooAbsReal* fracNP = nonPromptPDF.createIntegral(SIeta,NormSet(SIeta),Range("SR"));
    RooAbsReal* Inp=nonPromptPDF.createIntegral(SIeta);

    double f=Fitfrac.getVal();
    double fError=Fitfrac.getError();
    double dataAreaFull=hDataSR->Integral();
    double promptIntFull=f*dataAreaFull;
    double nonpromptIntFull=(1-f)*dataAreaFull;

    double fracP_val=fracP->getVal();
  
    double fracNP_val=fracNP->getVal();
  
    double promptIntS=fracP_val*f;
    double nonpromptIntS=fracNP_val*(1-f);
    double PurityFromFit=promptIntS/(promptIntS+nonpromptIntS);
  
    float SigmaErr=(fError/f)*PurityFromFit;

    cout<<"Beta error: "<<SigmaErr<<endl;

    cout<<"fracP:  "<<fracP_val<<endl;
    cout<<"fracNP:  "<<fracNP_val<<endl;
    cout<<"Purity From Fit: "<<PurityFromFit<<endl;

  


    char RfracFit[100];
    sprintf(RfracFit,"P(Fit to EB Data): %4.3f +- %4.3f ",PurityFromFit,SigmaErr); 
    tpav_txt->AddText(RfracFit);

  

  
    
    float chosenEff = 0.0107;
    TLine *line =new TLine(chosenEff,0,chosenEff,100);
    line->SetLineColor(kOrange+8);
    line->SetLineWidth(2);
    


   TCanvas *cFit=new TCanvas("cFit","cFit",850,850);
   
   cFit->cd();
   gPad->Update();
   gPad->SetLogy();
   cFit->Range(0.01,0.01,0.02,1000);
   frame4->Draw();
   tpav_txt->Draw();
   line->Draw();
   leg->Draw();
   cFit->SaveAs("PurityFitEB.png");
   cFit->SaveAs("PurityFitEB.pdf");
   cFit->SaveAs("PurityFitEB.gif");
   

}
