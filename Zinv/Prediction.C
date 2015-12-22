#include<iostream>

void Prediction(){


     TFile *f=new TFile("PhotonCR_input.root","READ");

     TH1F *h_Nobs_PhotonEB = (TH1F*)f->Get("hSearchBins_Data_EB");
     TH1F *h_Nobs_PhotonEE = (TH1F*)f->Get("hSearchBins_Data_EE");

  
     TH1F *hZinv =(TH1F*)f->Get("hSearchBins_Zinv");
     TH1F *hGJets =(TH1F*)f->Get("hSearchBins_GJets");


     TH1F *h_ZgR = (TH1F*)hZinv->Clone("h_ZgR");

     h_ZgR->Divide(hGJets); 

     double Purity=0.82;
     double PurityErr=0.08;
     double DoubleRatio=0.924;
 

     int nSearchBins=h_ZgR->GetSize()-2;
     




     const Int_t NumBins = nSearchBins;
     double binNumber[NumBins];
     double Yield[NumBins];
     double YieldErrUp[NumBins];
     double YieldErrLow[NumBins];

     double YieldMC[NumBins];
     double Yield_ErrUp_MC[NumBins];
     double Yield_ErrLow_MC[NumBins];

     double XErrLow[NumBins];
     double XErrUp[NumBins];




     for(int iBin=1;iBin<=nSearchBins;iBin++){

         double Nobs = h_Nobs_PhotonEB->GetBinContent(iBin) + h_Nobs_PhotonEE->GetBinContent(iBin);

         double ZGJetsRatio = h_ZgR->GetBinContent(iBin);

         double data_Prediction = DoubleRatio*ZGJetsRatio*Purity*Nobs; 

         cout<<"Bin "<<iBin<<" th:  "<<data_Prediction<<endl;



          }
     





} 
