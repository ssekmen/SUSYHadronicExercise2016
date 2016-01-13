
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


     hDYmData->Divide(hGJData);


     hDYmData->Draw("E");

    double DR=0;
    double DRerr=0;


    //here we are avergaing over 4 bins to get the double ratio. 4 bins because there is no stat beyond first 4 bins in the histogram.
    for(int i=1;i<=4;i++){

      DR=DR+hDYmData->GetBinContent(i);
   
      DRerr= DRerr+ hDYmData->GetBinError(i)*hDYmData->GetBinError(i);;
     


          }  
  

    double DoubleRatio=DR/4;
    double DoubleRatioErr=sqrt(DRerr);

    double RelativeErr=DoubleRatioErr/DoubleRatio;

    cout<<"Double Ratio: "<<DoubleRatio<<endl;
    cout<<"Error on Double ratio: "<<DoubleRatioErr<<endl;

    cout<<"Relative error on double ratio: "<<RelativeErr<<endl;

























}
