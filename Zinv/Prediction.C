#include<iostream>
//This is the code where we combine all pieces of information

double Error(double a,double x,double xerr,double y,double yerr,double z,double zerr){
double F=a*x*y*z;


double Ferr=a*sqrt((x*y*zerr)*(x*y*zerr) + (x*yerr*z)*(x*yerr*z) + (xerr*y*z)*(xerr*y*z));

return Ferr;

}




void Prediction(){


     TFile *f=new TFile("ControlRegion_Input.root","READ");
     TFile *fZgR=new TFile("ZGammaRatio.root","READ");
     //histograms to get the num of observed photons in the search bins
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



     //value of photon purity in six search bins for barrel and endcap photons respectively  
     //0.96 is the purity for low MHT region and 0.90 is the purity in the high MHT region
     double PurityEB[6]={0.96,0.96,0.90,0.96,0.96,0.90};
     double PurityEE[6]={0.96,0.96,0.90,0.96,0.96,0.90};

     //total uncertainties on purity for barel and endcap 
     //here the first one is stat error and second one is systematic error
     double pEBErrLowMHT=sqrt((0.006*0.006)+(0.03*0.03));//for barrel and low  MHT
     double pEEErrLowMHT=sqrt((0.006*0.006)+(0.046*0.046));//for endcap and low MHT

     double pEBErrHighMHT=sqrt((0.03*0.03)+(0.03*0.03));//for barrel and high MHT
     double pEEErrHighMHT=sqrt((0.03*0.03)+(0.046*0.046));//for endcap and high MHT


     //put the uncertanties into an array , so that it will be convenient to use it later
     double PurityEBerr[6]={pEBErrLowMHT,pEBErrLowMHT,pEBErrHighMHT,pEBErrLowMHT,pEBErrLowMHT,pEBErrHighMHT};
     double PurityEEerr[6]={pEEErrLowMHT,pEEErrLowMHT,pEEErrHighMHT,pEEErrLowMHT,pEEErrLowMHT,pEEErrHighMHT};

     //double ration for six bins with uncertainties has been put in arrays , so that it will be convenient later to use for calcualtion
     double DoubleRatio[6]={0.924,0.924,0.924,0.924,0.924,0.924};
     double DRerrUp[6]={0.12547,0.145487,0.365062,0.276206,0.365062,0.547004};
     double DRerrLow[6]={0.0796361,0.0967893,0.244409,0.183069,0.244409,0.371635};
     
   
     //photon direct fraction and corresponding uncertainties
     double Fdir=0.92;
     double FdirErr=0.07;


     int NumBins = 6;

   
    cout<<"Bin Nobs nEB  pEB  pEBerr nEC   pEC  pECerr  ZgR  REr1   RErUp RErLow  f   ferr     purityT(pErr)  DR     DRup  DRlow    Yield   YstatUp YstatLow YsysUp YsysLow "<<endl;


     //in this loop we will calculate the estimated Ztonunu events from data bin by bin
     for(int iBin=1;iBin<=NumBins;iBin++){//looping over bins

        int nEB=h_Nobs_PhotonEB->GetBinContent(iBin);//total number of barrel photons in data
        int nEE=h_Nobs_PhotonEE->GetBinContent(iBin);//total number of endcap photons in data
    
        int Nobs=nEB+nEE;//total number of observed photons in data  

        double ZgR=hZGammaR->GetBinContent(iBin);//Z/gamma Ratio with SF applied 
        double ZgRsysErrUp= hZGammaRsysErrUp->GetBinContent(iBin);//
        double ZgRsysErrLow= hZGammaRsysErrLow->GetBinContent(iBin);
        double ZgRstatErr = hZGammaRstatErr->GetBinContent(iBin);

        double ZgRErrUp=sqrt((ZgRsysErrUp*ZgRsysErrUp)+(ZgRstatErr*ZgRstatErr));//total up error on Z/Gamma Ratio

        double ZgRErrLow=sqrt((ZgRsysErrLow*ZgRsysErrLow)+(ZgRstatErr*ZgRstatErr));//total low error on Z/gamma Ratio

        double pEB=PurityEB[iBin-1];//purity barrel photon
        double pEE=PurityEE[iBin-1];//purity endcap photon
        double pEBerr=PurityEBerr[iBin-1];//purity error barrel photon
        double pEEerr=PurityEEerr[iBin-1];//purity error endcap photon
        double DR=DoubleRatio[iBin-1];//double ratio
        double DRupErr=DRerrUp[iBin-1];//double ratio error up
        double DRlowErr=DRerrLow[iBin-1];//double ratio error down
        double totalPurity=0;//total purity , combine photon purity + photon direct fraction in a consistent way
        double totalPurityErr=0;//error on total purity

        double YieldErr_SysUp=0;//variable for systematic up error on the prediction
        double YieldErr_SysLow=0;//variable for systematic low error on the prediction

        double YieldErr_StatUp=0; //variable for stat up error on the prediction
        double YieldErr_StatLow=0; //variable for stat low error on the prediction




        double data_Prediction = DR*ZgR*(nEB*pEB+nEE*pEE)*Fdir; //estimated Ztonunu event that would be present in the data
        
        if(Nobs !=0){//calculate the uncertaties in the bins with no of observed photons not equal to 0
   
          totalPurity=((nEB*pEB+nEE*pEE)*Fdir)/Nobs;
         
          //just to calculate the total uncertainty lets imagine totalPurity is product of following two parts
          //totalPurity=Purity1*Purity2
          //where Purity1=(nEB*pEB+nEE*pEE)/Nobs
          //Purity2=Fdir
          //
          double totalPurityErr1=(nEB*pEBerr+nEE*pEEerr)/Nobs;//error on Purity1
          double totalPurityErr2=FdirErr;//error on Purity2
          
          //total error on the totalPurity 
          totalPurityErr=sqrt((totalPurityErr1*totalPurityErr1)+(totalPurityErr2*totalPurityErr2));

           //systematic up error on the prediction//Error fucntion defined at the beginning of the code
           YieldErr_SysUp=Error(Nobs,DR,DRupErr,ZgR,ZgRErrUp,totalPurity,totalPurityErr);

           //systematic low error on the prediction//Error fucntion defined at the beginning of the code
           YieldErr_SysLow=Error(Nobs,DR,DRlowErr,ZgR,ZgRErrLow,totalPurity,totalPurityErr);

           //statistical up and low error on the prediction
           YieldErr_StatUp=DR*ZgR*totalPurity*(sqrt(Nobs));
           YieldErr_StatLow=DR*ZgR*totalPurity*(sqrt(Nobs));
          
           }

          
          if(Nobs ==0){//calculate the uncertainties in the bins with num of observed photons equal to zero
         
         //if the Nobs ==0 we cannot calculate the totalPurity , so in these cases we can use the numbers from GJets MC in place of Nobs 
         // to calculate the uncertianties but for the prediction central value we will not use GJets MC in place of Nobs
    
         double  NobsMC=h_BinYieldGJets_MC->GetBinContent(iBin);//Nobs from MC
         double  nEB_new=h_BinYieldGJets_MC_EB->GetBinContent(iBin);//Nobs from MC for barrel
         double nEE_new=h_BinYieldGJets_MC_EE->GetBinContent(iBin);//Bobs from MC for endcap


          //description of following line are the same as above as for the case Nobs not equal to 0 {
          totalPurity=((nEB_new*pEB+nEE_new*pEE)*Fdir)/NobsMC;
          
          double totalPurityErr1=(nEB_new*pEBerr+nEE_new*pEEerr)/NobsMC;
          double totalPurityErr2=FdirErr;
          totalPurityErr=sqrt((totalPurityErr1*totalPurityErr1)+(totalPurityErr2*totalPurityErr2));

          YieldErr_SysUp=Error(NobsMC,DR,DRupErr,ZgR,ZgRErrUp,totalPurity,totalPurityErr);

          YieldErr_SysLow=Error(NobsMC,DR,DRlowErr,ZgR,ZgRErrLow,totalPurity,totalPurityErr);

          YieldErr_StatUp=DR*ZgR*totalPurity*(1.8);//think why the poisson up error on 0 will be 1.8
          YieldErr_StatLow=DR*ZgR*totalPurity*(0.);//think why the poisson down error on 0 will be 0

          //}//description of following line are the same as above for case Nobs not equal to 0
           }
         
     
      
        // Prints the absolute error with prediction
        printf(" %i :%i |%i| %4.3f(%4.3f) |%i| %4.3f(%4.3f) |%4.3f(%4.3f,+%4.3f-%4.3f) |%4.3f(%4.3f) |%4.3f(%4.3f)| %4.3f(+%4.3f-%4.3f)| %4.3f(+%4.3f-%4.3f,+%4.3f-%4.3f)",iBin,Nobs, nEB,pEB,pEBerr,nEE,pEE,pEEerr, ZgR,ZgRstatErr,ZgRsysErrUp,ZgRsysErrUp,Fdir,FdirErr,totalPurity,totalPurityErr,DR,DRupErr,DRlowErr,data_Prediction,YieldErr_StatUp,YieldErr_StatLow,YieldErr_SysUp,YieldErr_SysLow);
       cout<<endl;




          }//looping over bins
     





} 
