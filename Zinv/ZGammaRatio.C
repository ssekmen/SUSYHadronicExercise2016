#include "SetStyle.C"
void ZGammaRatio(){

    gStyle->SetOptStat(0); 
    TFile *f=new TFile("ControlRegion_Input.root","READ"); 


    //here you access Zinv MC and GJets MC histograms needed for double
    TH1F *hZinv =(TH1F*)f->Get("hSearchBins_Zinv");
    TH1F *hGJets =(TH1F*)f->Get("hSearchBins_GJets");

    
    TH1F *h_ZgR = (TH1F*)hZinv->Clone("h_ZgR");
    h_ZgR->SetTitle("Z/Gamma Ratio");
    h_ZgR->GetXaxis()->SetTitle("bin Number");
    h_ZgR->GetYaxis()->SetTitle("(Z#rightarrow #nu#nu/#gamma +jets)_{MC} ");
    h_ZgR->GetYaxis()->SetTitleOffset(0.7);
    h_ZgR->Divide(hGJets);//divide the histograms to get the ZgammaRatio


    TPaveText *tpav1 = new TPaveText(0.5956522,0.447818,0.829097,0.8070332,"brNDC");

    tpav1->SetBorderSize(0);
    tpav1->SetFillStyle(0);
    tpav1->SetTextAlign(11);
    tpav1->SetTextFont(42);
    tpav1->SetTextSize(0.04);
    tpav1->AddText("H_{T} >500");
    tpav1->AddText("#gamma p_{T} > 100 ");
    tpav1->AddText("N_{jets} >=7");
    tpav1->AddText("H_{T}^{miss}>200");
    tpav1->AddText("N_{b-jets}>=2");
    tpav1->AddText("#Delta #Phi_{1,2,3,4}>(0.5,0.5,0.3,0.3)"); 


   TCanvas *c_ZgR=new TCanvas("ZGammaRatio","ZGammaRatio");
   c_ZgR->cd();
   h_ZgR->Draw("E");
   tpav1->Draw();

   c_ZgR->SaveAs("ZGammaRatiowithoutSF.png");



    //here you make a bunch of arrays to plot the Zgamma Ratio with corresponding uncertainties
    int nBins=6;
    double ZgRwSF[nBins];//ZgammaRatio with Data/MC SF applied
    double ZgRwSF_sys_ErrUp[nBins];//systematic error up on ZgammaRatio with Data/MC SF applied
    double ZgRwSF_sys_ErrLow[nBins];//systematic error low on ZgammaRatio with Data/MC SF applied

    double ZgRwSF_stat_Err[nBins];//stat error on Zgamma ratio with SF applied 
    double XErrLow[nBins];//these are just the bin sizes
    double XErrUp[nBins];//
    double binNumber[nBins];

    TFile *fZgR=new TFile("ZGammaRatio.root","RECREATE");
  
    TH1F *hZGammaR = new TH1F("hZGammaR","hZGammaR",6,0.5,6.5);
    TH1F *hZGammaRsysUp = new TH1F("hZGammaRsysUp","hZGammaRsysUp",6,0.5,6.5);
    TH1F *hZGammaRsysLow = new TH1F("hZGammaRsysLow","hZGammaRsysLow",6,0.5,6.5);
    TH1F *hZGammaRstat = new TH1F("hZGammaRstat","hZGammaRstat",6,0.5,6.5);


   for(int ibin=1;ibin<(nBins+1);ibin++){//loop over bin error calculation


       double SF=0.99;//This is the Data/MC scale factor for ID and Isolation with uncertainty 0.05
       double SFup=1.04;//SF one sigma up 0.99+0.05
       double SFlow=0.94; //SF one sigma down 0.99-0.05
       double ZgR_woSF=h_ZgR->GetBinContent(ibin);//
       double ZgRcentral=h_ZgR->GetBinContent(ibin)/SF;//central value of ZgammaRatio with Data/MC SF applied  
       double ZgRup=h_ZgR->GetBinContent(ibin)/SFup;//Zgamma Ratio scaled with Data/MC SFup
       double ZgRlow=h_ZgR->GetBinContent(ibin)/SFlow;//Zgamma Ratio scaled with Data/MC SFlow

      

       double ZgRsysErrup=fabs(ZgRcentral-ZgRlow);//systematic up error on the ZgRcentral
       double ZgRsysErrlow=fabs(ZgRcentral-ZgRup);//systematic low error on the ZgRcentral
       double ZgRstatErr=h_ZgR->GetBinError(ibin)/SF;//statistical error on ZgRcentral


       cout<<"Bin Num "<<ibin<<":  "<<"ZgR_WO_SF: "<<ZgR_woSF<<"    ZgR_W_SF: "<<ZgRcentral<<endl;

       binNumber[ibin-1]=ibin;
       ZgRwSF[ibin-1]=ZgRcentral;
       ZgRwSF_sys_ErrUp[ibin-1]=ZgRsysErrup;
       ZgRwSF_sys_ErrLow[ibin-1]=ZgRsysErrlow;
       ZgRwSF_stat_Err[ibin-1]=ZgRstatErr;
       XErrLow[ibin-1]=0.5;
       XErrUp[ibin-1]=0.5;

      hZGammaR->SetBinContent(ibin, ZgRcentral);
      hZGammaRsysUp->SetBinContent(ibin, ZgRsysErrup);
      hZGammaRsysLow->SetBinContent(ibin, ZgRsysErrlow);
      hZGammaRstat->SetBinContent(ibin, ZgRstatErr);



   }


    fZgR->Write();


     TCanvas *c_ZgRwSF = new TCanvas("c_ZgRwSF","c_ZgRwSF");
     c_ZgRwSF->cd();


    //see that the central value in two TGraphAssymetric errors are same .
     TGraphAsymmErrors *gr_ZgRwSFsys = new TGraphAsymmErrors(nBins,binNumber,ZgRwSF,XErrLow,XErrUp,ZgRwSF_sys_ErrLow,ZgRwSF_sys_ErrUp);
     gr_ZgRwSFsys->SetTitle("ZgammaRatio");
     gr_ZgRwSFsys->SetMarkerColor(4);
     gr_ZgRwSFsys->SetLineColor(4);
     gr_ZgRwSFsys->SetMarkerStyle(21);

     TGraphAsymmErrors *gr_ZgRwSFstat = new TGraphAsymmErrors(nBins,binNumber,ZgRwSF,XErrLow,XErrUp,ZgRwSF_stat_Err,ZgRwSF_stat_Err);
     gr_ZgRwSFstat->SetTitle("ZgammaRatio");
     gr_ZgRwSFstat->SetMarkerColor(2);
     gr_ZgRwSFstat->SetLineColor(2);
     gr_ZgRwSFstat->SetMarkerStyle(21);


      TLegend *legPZgR = new TLegend(0.4173913,0.7478185,0.5167224,0.8563002,NULL,"brNDC");
      legPZgR->SetBorderSize(0);
      legPZgR->SetTextFont(62);
      legPZgR->SetTextSize(0.03);
      legPZgR->SetLineStyle(1);
      legPZgR->SetLineWidth(3);
      legPZgR->SetFillColor(0);
      legPZgR->SetFillStyle(1001);

      legPZgR->AddEntry(gr_ZgRwSFsys,"Sys Error","l");
      legPZgR->AddEntry(gr_ZgRwSFstat,"Stat Error","l");

      TMultiGraph *mgZgR=new TMultiGraph();
      mgZgR->SetTitle(" ;ith Bin ; Z/#gamma Ratio with SF applied ");

      mgZgR->Add(gr_ZgRwSFstat);
      mgZgR->Add(gr_ZgRwSFsys);

      mgZgR->Draw("AP");
      tpav1->Draw();
      legPZgR->Draw();


     c_ZgRwSF->SaveAs("ZGammaRatioWithSF.png");









}
