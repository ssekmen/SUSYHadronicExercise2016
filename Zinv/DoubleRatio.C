#include "SetStyle.C"

void DoubleRatio(){

    //Sets the style of Canvas ,it is like the tdrStyle function
    gStyle->SetOptStat(0);

    TFile *f=new TFile("ControlRegion_Input.root","READ"); 

    TH1F *hGJ=(TH1F*)f->Get("hNJets_GJets");
    TH1F *hGJData=(TH1F*)f->Get("hNJets_DataGJ");

    TH1F *hDYm=(TH1F*)f->Get("hNJets_DYm");
    hDYm->Scale(1/1.23);



    TH1F *hDYmData=(TH1F*)f->Get("hNJets_DataDYm");
    hDYmData->SetTitle("Double Ratio");
    hDYmData->GetXaxis()->SetTitle("N_{jets}");
    hDYmData->GetXaxis()->SetTitleOffset(0.70);
    hDYmData->GetYaxis()->SetTitle("Double ratio");
    hDYmData->GetYaxis()->SetTitleOffset(0.70);


    hGJData->Divide(hGJ);

    hDYmData->Divide(hDYm);

  // TEfficiency *pEff = new TEfficiency(hDYmData,hGJData);

    hDYmData->Divide(hGJData);


    hDYmData->Draw("E");

    






















}
