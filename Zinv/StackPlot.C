#include<iostream>
#include "TStyle.h"
using namespace std;

class DataMC{

public:

DataMC();

void drawLegend(TH1F *h_1=0,TH1F *h_2=0,TH1F *h_3=0,std::string H1R="",std::string H2R="",std::string H3R="");
void Stack(TH1F *h1=0,TH1F *h2=0,TH1F *hnp=0,TCanvas *c=0,std::string Region="",std::string h1R="",std::string h2R="",std::string variable="");



};


DataMC::DataMC(){


}


void DataMC::drawLegend(TH1F *h_1=0,TH1F *h_2=0,TH1F *h_3=0,std::string H1R="",std::string H2R="",std::string H3R=""){

char h1L[100];
char h2L[100];
char h3L[100];  
const char *h1_L;
const char *h2_L;
const char *h3_L;

h1_L=H1R.c_str();
h2_L=H2R.c_str();
h3_L=H3R.c_str();

sprintf(h1L,"%s",h1_L);
sprintf(h2L,"%s",h2_L);
sprintf(h3L,"%s",h3_L);

TLegend *leg=new TLegend(0.3582943,0.7051483,0.55025753,0.9049564,NULL,"brNDC");
leg->SetTextFont(62);
leg->SetLineColor(1);
leg->SetLineStyle(1);
leg->SetLineWidth(3);
leg->SetFillColor(0);
leg->SetFillStyle(1001);
leg->SetShadowColor(0);
leg->SetDrawOption(0);
leg->SetBorderSize(0);
leg->SetTextSize(0.03);


leg->AddEntry(h_1,h1L,"P");
leg->AddEntry(h_2,h2L,"f");
leg->AddEntry(h_3,h3L,"f");
//c->cd(
leg->Draw();




}












void DataMC::Stack(TH1F *h1,TH1F *h2,TH1F *hnp,TCanvas *c,std::string Region="",std::string h1R="",std::string h2R="",std::string variable=""){



    char h1L[100];
    char h2L[100];

    char region[100];
    char PlotName[100];  

    char Ratio[100];
    const char *h1_L;
    const char *h2_L;
    const char *Rgn;
    const char *variable_;
    variable_=variable.c_str();
    char VariableName[100];
    sprintf(VariableName,"%s",variable_);


    std::string plotstring=variable+"_"+"_"+h1R+"_"+h2R+"_"+Region;

    const char *plotname;
  
    plotname=plotstring.c_str();


    h1_L=h1R.c_str();
    h2_L=h2R.c_str();
    Rgn=Region.c_str();

    sprintf(h1L,"%s",h1_L);
    sprintf(h2L,"%s",h2_L);  

    sprintf(Ratio,"%s/%s",h1_L,h2_L);
    sprintf(region,"%s",Rgn);

    double scaleFactor=h1->Integral()/(h2->Integral()+hnp->Integral());
    
    cout<<"Data/MC: "<<h1->Integral()/(h2->Integral()+hnp->Integral())<<endl;  

    TH1F *hAllData=(TH1F*)h1->Clone("hAllData");
    TH1F *hAllMC=(TH1F*)h2->Clone("hAllMC");
    hAllMC->Add(hnp);

    double x=hAllData->Integral();
    double y=hAllMC->Integral();
    double xErr=0;
    double yErr=0;
    int xMaxBin=hAllData->GetSize()-2;
    int yMaxBin=hAllMC->GetSize()-2;

    hAllData->IntegralAndError(0,xMaxBin,xErr);
    hAllMC->IntegralAndError(0,yMaxBin,yErr);
    double SFErr=(1/(y*y))*sqrt((y*y*xErr*xErr)+(x*x*yErr*yErr));










    char dataMCSF[100];
    sprintf(dataMCSF,"Data/MC= %4.2f +/- %4.2f",scaleFactor,SFErr);




    TPaveText *tpav_txt = new TPaveText(0.57043478,0.508342,0.8652174,0.9210471,"brNDC");

    tpav_txt->SetBorderSize(0);
    tpav_txt->SetFillStyle(0);
    tpav_txt->SetTextAlign(11);
    tpav_txt->SetTextFont(42);
    tpav_txt->SetTextSize(0.04);
    tpav_txt->AddText("H_{T} >500");
    tpav_txt->AddText("#gamma p_{T} > 100 ");
    tpav_txt->AddText("N_{jets} >=7");
    tpav_txt->AddText("H_{T}^{miss}>800");
   // tpav_txt->AddText("N_{b-jets}=0");
    tpav_txt->AddText("#Delta #Phi_{1,2,3,4}>(0.5,0.5,0.3,0.3)");
    tpav_txt->AddText(region);
    tpav_txt->AddText(dataMCSF);
    

    



   c=new TCanvas("MyCanvas","MyCanvas",600,700);
   TPad *pad1 = new TPad("pad1", "pad1", 0, 0.3, 1, 1.0);
   pad1->SetBottomMargin(0); // Upperand lower plot are joined
   //pad1->SetGridy();         // Vertical grid
   pad1->Draw();             // Draw the upper pad: pad1
   pad1->cd();               // pad1 becomes the current pad
   
   gPad->SetLogy();

   
   
   THStack *hs=new THStack("hs","hs");
   TH1F *hP =new TH1F(*h2);
   hP->SetFillColor(kOrange+8);
   hP->SetFillStyle(1000);
   hnp->SetFillColor(kOrange+4);
   hnp->SetFillStyle(1000);
   

   hs->Add(hnp);
   hs->Add(hP);
   //cout<<"Prompt: "<<hP->Integral()<<endl;
   //cout<<"non Prompt: "<<hnp->Integral()<<endl;
   
   
   TPaveText *pCMS = new TPaveText(0.132107,0.9308003,0.8327759,0.9923583,"brNDC");

    pCMS->SetBorderSize(0);
    pCMS->SetFillStyle(0);
    pCMS->SetTextAlign(11);
    pCMS->SetTextFont(42);
    pCMS->SetTextSize(0.04);
    pCMS->AddText("CMS #it{Preliminary}                   2.1 fb^{-1}, #sqrt{s}= 13 TeV");    


   hs->SetMinimum(0.1);
   hs->SetMaximum(hs->GetMaximum()*2500);
   hs->Draw("hist");
   hs->GetYaxis()->SetTitle("Events");
   hs->Draw("hist");
   
   h1->Draw("P,E1 SAME");
   tpav_txt->Draw();
   pCMS->Draw();
   drawLegend(h1,hP,hnp,"Data","#gamma+Jets","QCD");


    
    h2->Add(hnp);
    h2->Sumw2();




   TGaxis *axis = new TGaxis( -5, 20, -5, 220, 20,220,510,"");
   axis->SetLabelFont(43); // Absolute font size in pixel (precision 3)
   axis->SetLabelSize(15);
   axis->Draw();


   c->cd();

   TPad *pad2 = new TPad("pad2", "pad2", 0, 0.05, 1, 0.3);
   pad2->SetTopMargin(0);
   pad2->SetBottomMargin(0.2);
   pad2->SetGridy(); // vertical grid
   pad2->Draw();
   pad2->cd();       // pad2 becomes the current pad

   // Define the ratio plot
   TH1F *h3 = (TH1F*)h1->Clone("h3");
   h3->SetLineColor(kBlack);
   h3->SetMinimum(0);  // Define Y ..
   h3->SetMaximum(2); // .. range
   h3->Sumw2();
   h3->SetStats(0);      // No statistics on lower plot
   h3->Divide(h2);
   h3->SetMarkerStyle(21);
   h3->Draw("ep");       // Draw the r


   // h1 settings
   //h1->SetLineColor(kBlue);
   h1->SetMarkerStyle(20);
   h1->SetMarkerColor(1);
   h1->SetLineWidth(1);

   // Y axis h1 plot settings
   h1->GetYaxis()->SetTitleSize(20);
   h1->GetYaxis()->SetTitleFont(43);
   h1->GetYaxis()->SetTitleOffset(1.55);

   // h2 settings
   h2->SetLineColor(kRed);
   h2->SetLineWidth(2);
   h2->SetMarkerStyle(8);
   h2->SetMarkerColor(kRed);

   // Ratio plot (h3) settings
   
   h3->GetXaxis()->SetTitle(VariableName); // Remove the ratio title

   // Y axis ratio plot settings
   h3->GetYaxis()->SetTitle(Ratio);
   h3->GetYaxis()->SetNdivisions(505);

   h3->GetYaxis()->SetTitleSize(20);
   h3->GetYaxis()->SetTitleFont(43);
   h3->GetYaxis()->SetTitleOffset(1.55);
   h3->GetYaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
   h3->GetYaxis()->SetLabelSize(15);

   // X axis ratio plot settings
   h3->GetXaxis()->SetTitleSize(20);
   h3->GetXaxis()->SetTitleFont(43);
   h3->GetXaxis()->SetTitleOffset(3.);
   h3->GetXaxis()->SetLabelFont(43); // Absolute font size in pixel (precision 3)
   h3->GetXaxis()->SetLabelSize(15);








char pName[100];

sprintf(pName,"%s.png",plotname);
c->SaveAs(pName);
sprintf(pName,"%s.gif",plotname);
c->SaveAs(pName);
sprintf(pName,"%s.pdf",plotname);
c->SaveAs(pName);


}
