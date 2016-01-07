#include<iostream>


double Error(double a,double x,double xerr,double y,double yerr,double z,double zerr){
double F=a*x*y*z;


double Ferr=a*sqrt((x*y*zerr)*(x*y*zerr) + (x*yerr*z)*(x*yerr*z) + (xerr*y*z)*(xerr*y*z));

return Ferr;

}




void Prediction(){


     TFile *f=new TFile("ControlRegion_Input.root","READ");
     TFile *fZgR=new TFile("ZGammaRatio.root","READ");

     TH1F *h_Nobs_PhotonEB = (TH1F*)f->Get("hSearchBins_DataGJ_EB");
     TH1F *h_Nobs_PhotonEE = (TH1F*)f->Get("hSearchBins_DataGJ_EE");
     TH1F *h_Zinv = (TH1F*)f->Get("hSearchBins_Zinv");
     h_Zinv->Scale(1.23);//scaled by k factor

     TH1F *h_BinYieldGJets_MC = (TH1F*)f->Get("hSearchBins_GJets");
     TH1F *h_BinYieldGJets_MC_EB = (TH1F*)f->Get("hSearchBins_GJets_EB");
     TH1F *h_BinYieldGJets_MC_EE = (TH1F*)f->Get("hSearchBins_GJets_EE");

 
     TH1F *hZGammaR = (TH1F*)fZgR->Get("hZGammaR");
     TH1F *hZGammaRsysErrUp = (TH1F*)fZgR->Get("hZGammaRsysUp");
     TH1F *hZGammaRsysErrLow = (TH1F*)fZgR->Get("hZGammaRsysLow");
     TH1F *hZGammaRstatErr = (TH1F*)fZgR->Get("hZGammaRstat"); 




     double PurityEB[6]={0.96,0.96,0.90,0.96,0.96,0.90};
     double PurityEE[6]={0.96,0.96,0.90,0.96,0.96,0.90};

     double pEBErrLowMHT=sqrt((0.006*0.006)+(0.03*0.03));
     double pEEErrLowMHT=sqrt((0.006*0.006)+(0.046*0.046));

     double pEBErrHighMHT=sqrt((0.03*0.03)+(0.03*0.03));
     double pEEErrHighMHT=sqrt((0.03*0.03)+(0.046*0.046));


     double PurityEBerr[6]={pEBErrLowMHT,pEBErrLowMHT,pEBErrHighMHT,pEBErrLowMHT,pEBErrLowMHT,pEBErrHighMHT};
     double PurityEEerr[6]={pEEErrLowMHT,pEEErrLowMHT,pEEErrHighMHT,pEEErrLowMHT,pEEErrLowMHT,pEEErrHighMHT};


     double DoubleRatio[6]={0.924,0.924,0.924,0.924,0.924,0.924};
     double DRerrUp[6]={0.12547,0.145487,0.365062,0.276206,0.365062,0.547004};
     double DRerrLow[6]={0.0796361,0.0967893,0.244409,0.183069,0.244409,0.371635};
     
    
     double Fdir=0.92;
     double FdirErr=0.07;


     int NumBins = 6;


     for(int iBin=1;iBin<=NumBins;iBin++){//looping over bins

        int nEB=h_Nobs_PhotonEB->GetBinContent(iBin);
        int nEE=h_Nobs_PhotonEE->GetBinContent(iBin);
    
        int Nobs=nEB+nEE;   

        double ZgR=hZGammaR->GetBinContent(iBin);
        double ZgRsysErrUp= hZGammaRsysErrUp->GetBinContent(iBin);
        double ZgRsysErrLow= hZGammaRsysErrLow->GetBinContent(iBin);
        double ZgRstatErr = hZGammaRstatErr->GetBinContent(iBin);

        double ZgRErrUp=sqrt((ZgRsysErrUp*ZgRsysErrUp)+(ZgRstatErr*ZgRstatErr));

        double ZgRErrLow=sqrt((ZgRsysErrLow*ZgRsysErrLow)+(ZgRstatErr*ZgRstatErr));

        double pEB=PurityEB[iBin-1];
        double pEE=PurityEE[iBin-1];
        double pEBerr=PurityEBerr[iBin-1];
        double pEEerr=PurityEEerr[iBin-1];
        double DR=DoubleRatio[iBin-1];
        double DRupErr=DRerrUp[iBin-1];
        double DRlowErr=DRerrLow[iBin-1];
        double totalPurity=0;
        double totalPurityErr=0;

        double YieldErr_SysUp=0;
        double YieldErr_SysLow=0;
        double YieldErr_StatUp=0;
        double YieldErr_StatLow=0;




        double data_Prediction = DR*ZgR*(nEB*pEB+nEE*pEE)*Fdir; 
        
        if(Nobs !=0){
   
          totalPurity=((nEB*pEB+nEE*pEE)*Fdir)/Nobs;
         
          double totalPurityErr1=(nEB*pEBerr+nEE*pEEerr)/Nobs;
          double totalPurityErr2=FdirErr;
          totalPurityErr=sqrt((totalPurityErr1*totalPurityErr1)+(totalPurityErr2*totalPurityErr2));

           YieldErr_SysUp=Error(Nobs,DR,DRupErr,ZgR,ZgRErrUp,totalPurity,totalPurityErr);

           YieldErr_SysLow=Error(Nobs,DR,DRlowErr,ZgR,ZgRErrLow,totalPurity,totalPurityErr);

           YieldErr_StatUp=DR*ZgR*totalPurity*(sqrt(Nobs));
           YieldErr_StatLow=DR*ZgR*totalPurity*(sqrt(Nobs));
          
           }

          
          if(Nobs ==0){
         
         double  NobsMC=h_BinYieldGJets_MC->GetBinContent(iBin);
         double  nEB_new=h_BinYieldGJets_MC_EB->GetBinContent(iBin);
         double nEE_new=h_BinYieldGJets_MC_EE->GetBinContent(iBin);

          totalPurity=((nEB_new*pEB+nEE_new*pEE)*Fdir)/NobsMC;
          
          double totalPurityErr1=(nEB_new*pEBerr+nEE_new*pEEerr)/NobsMC;
          double totalPurityErr2=FdirErr;
          totalPurityErr=sqrt((totalPurityErr1*totalPurityErr1)+(totalPurityErr2*totalPurityErr2));

          YieldErr_SysUp=Error(NobsMC,DR,DRupErr,ZgR,ZgRErrUp,totalPurity,totalPurityErr);

          YieldErr_SysLow=Error(NobsMC,DR,DRlowErr,ZgR,ZgRErrLow,totalPurity,totalPurityErr);

          YieldErr_StatUp=DR*ZgR*totalPurity*(1.8);
          YieldErr_StatLow=DR*ZgR*totalPurity*(0.);
           }
         
      

        printf(" %i :%i |%i| %4.3f(%4.3f) |%i| %4.3f(%4.3f) |%4.3f(%4.3f,+%4.3f-%4.3f) |%4.3f(%4.3f) |%4.3f(%4.3f)| %4.3f(+%4.3f-%4.3f)| %4.3f(+%4.3f-%4.3f,+%4.3f-%4.3f)",iBin,Nobs, nEB,pEB,pEBerr,nEE,pEE,pEEerr, ZgR,ZgRstatErr,ZgRsysErrUp,ZgRsysErrUp,Fdir,FdirErr,totalPurity,totalPurityErr,DR,DRupErr,DRlowErr,data_Prediction,YieldErr_StatUp,YieldErr_StatLow,YieldErr_SysUp,YieldErr_SysLow);
       cout<<endl;




          }//looping over bins
     





} 
