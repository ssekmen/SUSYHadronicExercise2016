#include <vector>
#include <cstdio>
#include <iostream>   // std::cout
#include <string>     // std::string, std::to_string
#include <tdrstyle.C>
#include <CMS_lumi.C>


using namespace std;

/*

root.exe -b -q 'Plot_searchBin_full.C("stacked","searchH_b","Elog408_","Elog404_")'
root.exe -b -q 'Plot_searchBin_full.C("stacked","QCD_Low",  "Elog408_","Elog404_")'
root.exe -b -q 'Plot_searchBin_full.C("stacked","QCD_Up",   "Elog408_","Elog404_")'

.L Plot_searchBin_full.C
Plot_searchBin_full("stacked","searchH_b","Elog365_");
Plot_searchBin_full("stacked","QCD_Low","Elog365_");
Plot_searchBin_full("stacked","QCD_Up","Elog365_");

root.exe -b -q 'Plot_searchBin_full.C("stacked","searchH_b","Elog365_")'
root.exe -b -q 'Plot_searchBin_full.C("stacked","QCD_Low","Elog365_")'
root.exe -b -q 'Plot_searchBin_full.C("stacked","QCD_Up","Elog365_")'

*/

void shift_bin(TH1* input, TH1* output){

  char tempname[200];  
  char temptitle[200];  
  output->SetName(tempname);
  output->SetTitle(temptitle);
  output->SetBins(input->GetNbinsX(),input->GetBinLowEdge(1)-0.5,input->GetBinLowEdge(input->GetNbinsX()+1)-0.5);
  //input->Print("all");
  //output = new TH1D(tempname,temptitle,input->GetNbinsX(),input->GetBinLowEdge(1)-0.5,input->GetBinLowEdge(input->GetNbinsX()+1)-0.5); 
  // 0: underflow
  // 1: first bin [Use the lowedge of this bin]
  // input->GetNbinsX(): highest bin 
  // input->GetNbinsX()+1: overflow bin [use the lowedge of this bin]
  //

  for (int ibin=1;ibin<=input->GetNbinsX();ibin++){
    output->SetBinContent(ibin,input->GetBinContent(ibin));    
    output->SetBinError(ibin,input->GetBinError(ibin));    
    //std::cout << input->GetBinContent(ibin) << std::endl;
  }

}

void PlotClosure(string option="", int pull=0){ // string option="QCD"

  // Use option="QCD" to produce plots in QCD binning

  char tempname[200];
  // Open root file
  sprintf(tempname,"LLPrediction.root");

  // false: do closure test (MC prediction vs MC truth)
  // true: do data driven prediction and compare to MC truth
  bool doDataVsMC = false;

  // Add systematics in quadrature to stat. uncertainty on prediction
  // Non-closure systematic not included yet!
  bool showSystematics = false;

  ///////////////////////////////////////////////////////////////////////////////////////////
  ////Some cosmetic work for official documents.
  //
  // Set basic style
  //
  setTDRStyle();
  gStyle->SetPalette(1) ; // for better color output

  //
  // Canvas size
  int W = 1200;
  int H = 740;
  int H_ref = 740;
  int W_ref = 800;
  float T = 0.10*H_ref;
  float B = 0.06*H_ref;
  float L = 0.16*W_ref;
  float R = 0.04*W_ref;

  //
  // Various vertical line coordinates
  float ymax_top = 40000.;
  float ymin_top = 0.015;

  float ymax2_top = 1000.;
  float ymax3_top = 200.;
  float ymax4_top = 30.;

  float ymax_bottom = 1.99;
  float ymin_bottom = 0.01;

  float ymax2_bottom = 2.15;
  float ymax3_bottom = 2.15;
  float ymax4_bottom = 2.15;

  //
  // Luminosity information for scaling
  double lumi     = 2.3; // normaliza to this lumi (fb-1)
  double lumi_ref = 2.3; // normaliza to 3 (fb-1)
  
  ///////////////////////////////////////////////////////////////////////////////////////////
  //
  // More specific style set, opening input files etc

  gStyle->SetOptStat(0);  ///to avoid the stat. on the plots
  //gStyle->SetErrorX(0);
  char xtitlename[200];
  char ytitlename[200];

  TFile * LLFile = new TFile(tempname,"R");
  printf("Opened %s\n",tempname);

  //
  // Define legend
  //
  Float_t legendX1 = .65; //.50;
  Float_t legendX2 = .95; //.70;
  Float_t legendY1 = .57; //.6;
  Float_t legendY2 = .82;

  TLegend* catLeg1 = new TLegend(legendX1,legendY1,legendX2,legendY2);
  //catLeg1->SetTextSize(0.060);
  catLeg1->SetTextSize(0.044);
  catLeg1->SetTextFont(42);
  catLeg1->SetFillColor(0);
  catLeg1->SetLineColor(1);
  catLeg1->SetBorderSize(1);

  //
  // Define canvas
  //
  TCanvas *canvas = new TCanvas("canvas","canvas",10,10,W,H);

  canvas->SetFillColor(0);
  canvas->SetBorderMode(0);
  canvas->SetFrameFillStyle(0);
  canvas->SetFrameBorderMode(0);
  canvas->SetLeftMargin( L/W );
  canvas->SetRightMargin( R/W );
  canvas->SetTopMargin( T/H );
  canvas->SetBottomMargin( B/H );
  canvas->SetTickx(0);
  canvas->SetTicky(0);

  canvas->Divide(1, 2);
  
  //
  // Define pads
  //
  TPad* canvas_up = (TPad*) canvas->GetListOfPrimitives()->FindObject("canvas_1");
  TPad* canvas_dw = (TPad*) canvas->GetListOfPrimitives()->FindObject("canvas_2");

  //
  // define the size
  double up_height     = 0.8;  // please tune so that the upper figures size will meet your requirement
  double dw_correction = 1.30; // please tune so that the smaller canvas size will work in your environment
  double font_size_dw  = 0.1;  // please tune the font size parameter for bottom figure
  double dw_height     = (1. - up_height) * dw_correction;
  double dw_height_offset = 0.04; // KH, added to put the bottom one closer to the top panel

  //
  // set pad size
  canvas_up->SetPad(0., 1 - up_height,    1., 1.00);
  canvas_dw->SetPad(0., 0.,               1., dw_height+dw_height_offset);
  //
  canvas_up->SetFrameFillColor(0);
  canvas_up->SetFillColor(0);
  canvas_up->SetTopMargin(0.12);
  canvas_up->SetLeftMargin(0.1);
  //
  canvas_dw->SetFillColor(0);
  canvas_dw->SetFrameFillColor(0);
  canvas_dw->SetBottomMargin(0.35);
  canvas_dw->SetTopMargin(0);
  canvas_dw->SetLeftMargin(0.1);
  
  //
  // draw top figure
  canvas_up->cd();

  TH1D * GenHist, * EstHist,* thist;
  TH1D * GenHistTemp, * EstHistTemp;
  TH1D * GenHistD, * EstHistD;
  TH1D * GenHistDTemp, * EstHistDTemp;
  TH1D * EstSystematics;
  TH1D * histTemplate;

  double search_x_max=6.5;
  double search_x_min=0.5;

  TDirectory *dPre = 0;
  TDirectory *dExp = (TDirectory*)LLFile->Get("Expectation");

  if(doDataVsMC){
    dPre = (TDirectory*)LLFile->Get("Prediction_data");
  }else{
    dPre = (TDirectory*)LLFile->Get("Prediction_MC");
  }  

  if(doDataVsMC){
    EstHistTemp=(TH1D*) dPre->Get("totalPred_LL")->Clone();
    EstHistDTemp=(TH1D*) dPre->Get("totalPred_LL")->Clone();
  }else{
    EstHistTemp=(TH1D*) dPre->Get("totalPred_LL_MC")->Clone();
    EstHistDTemp=(TH1D*) dPre->Get("totalPred_LL_MC")->Clone();
  }

  GenHistTemp=(TH1D*) dExp->Get("totalExp_LL")->Clone();
  GenHistDTemp=(TH1D*) dExp->Get("totalExp_LL")->Clone();

  if(showSystematics){
    TDirectory *dSys = (TDirectory*)LLFile->Get("AdditionalContent");
    if(doDataVsMC){
      EstSystematics=(TH1D*) dSys->Get("totalPropSysUp_LL")->Clone();
    }else{
      EstSystematics=(TH1D*) dSys->Get("totalPropSysUp_LL_MC")->Clone();
    }
  }

  if(EstHistTemp->GetNbinsX() != GenHistTemp->GetNbinsX()) std::cout<<"NbinsX of Expectation and Prediction don't agree!"<<std::endl;

  EstHist = new TH1D("Exp", "Exp", EstHistTemp->GetNbinsX(), 0.5, EstHistTemp->GetNbinsX()+0.5);
  EstHistD = new TH1D("ExpD", "ExpD", EstHistDTemp->GetNbinsX(), 0.5, EstHistDTemp->GetNbinsX()+0.5);
  GenHist = new TH1D("Pred", "Pred", GenHistTemp->GetNbinsX(), 0.5, GenHistTemp->GetNbinsX()+0.5);
  GenHistD = new TH1D("PredD", "PredD", GenHistDTemp->GetNbinsX(), 0.5, GenHistDTemp->GetNbinsX()+0.5);

  for(int i = 0; i <= EstHistTemp->GetNbinsX(); i++){
    EstHist->SetBinContent(i, EstHistTemp->GetBinContent(i));    
    EstHistD->SetBinContent(i, EstHistDTemp->GetBinContent(i));

    if(showSystematics){
      EstHist->SetBinError(i, std::sqrt(EstHistTemp->GetBinError(i)*EstHistTemp->GetBinError(i)+EstSystematics->GetBinContent(i)*EstSystematics->GetBinContent(i)*EstHistTemp->GetBinError(i)*EstHistTemp->GetBinError(i)));
      EstHistD->SetBinError(i, std::sqrt(EstHistDTemp->GetBinError(i)*EstHistDTemp->GetBinError(i)+EstSystematics->GetBinContent(i)*EstSystematics->GetBinContent(i)*EstHistDTemp->GetBinError(i)*EstHistDTemp->GetBinError(i)));
    }else{
      EstHist->SetBinError(i, EstHistTemp->GetBinError(i));
      EstHistD->SetBinError(i, EstHistDTemp->GetBinError(i));
    }    

    GenHist->SetBinContent(i, GenHistTemp->GetBinContent(i));
    GenHist->SetBinError(i, GenHistTemp->GetBinError(i));
    GenHistD->SetBinContent(i, GenHistDTemp->GetBinContent(i));
    GenHistD->SetBinError(i, GenHistDTemp->GetBinError(i));
  }

  GenHist->SetLineColor(4);
  EstHist->SetLineColor(4);
  //GenHist->GetXaxis()->SetLabelFont(42);
  //GenHist->GetXaxis()->SetLabelOffset(0.007);
  //GenHist->GetXaxis()->SetLabelSize(0.04);
  //GenHist->GetXaxis()->SetTitleSize(0.05);
  //GenHist->GetXaxis()->SetTitleOffset(0.9);
  //GenHist->GetXaxis()->SetTitleOffset(0.5);
  //GenHist->GetXaxis()->SetTitleFont(42);
  //GenHist->GetYaxis()->SetLabelFont(42);
  //GenHist->GetYaxis()->SetLabelOffset(0.007);
  //GenHist->GetYaxis()->SetLabelSize(0.04);
  GenHist->GetYaxis()->SetLabelSize(0.045*1.15);
  GenHist->GetYaxis()->SetTitleSize(0.06*1.15);
  GenHist->GetYaxis()->SetTitleOffset(0.6);
  GenHist->GetYaxis()->SetTitleFont(42);


  //EstHist->GetXaxis()->SetLabelFont(42);
  //EstHist->GetXaxis()->SetLabelOffset(0.007);
  //EstHist->GetXaxis()->SetLabelSize(0.04);
  //EstHist->GetXaxis()->SetTitleSize(0.05);
  //EstHist->GetXaxis()->SetTitleOffset(0.9);
  //EstHist->GetXaxis()->SetTitleFont(42);
  //EstHist->GetYaxis()->SetLabelFont(42);
  //EstHist->GetYaxis()->SetLabelOffset(0.007);
  //EstHist->GetYaxis()->SetLabelSize(0.04);
  //EstHist->GetYaxis()->SetTitleSize(0.08);
  //EstHist->GetYaxis()->SetTitleOffset(2.0);
  //EstHist->GetYaxis()->SetTitleFont(42);
  //  sprintf(xtitlename,"Search region bin number");
  // label the x-axis with the search bin definitions
  // 7 #leq N_{jet} #leq 8
   TString cut_labels[6] = {"#splitline{N_{b-jet} = 2}{200 < H_{T}^{ miss} < 500}","#splitline{N_{b-jet} #geq 3}{200 < H_{T}^{ miss} < 500}","#splitline{N_{b-jet} #geq 2}{H_{T}^{ miss} > 500}",
			"#splitline{N_{b-jet} = 2}{200 < H_{T}^{ miss} < 500}","#splitline{N_{b-jet} #geq 3}{200 < H_{T}^{ miss} < 500}","#splitline{N_{b-jet} #geq 2}{H_{T}^{ miss} > 500}"};
  sprintf(xtitlename,"");
  sprintf(ytitlename,"Events");
  gPad->SetLogy();
  GenHist->SetMaximum(ymax_top);
  GenHist->SetMinimum(ymin_top);
  GenHist->GetXaxis()->SetRangeUser(search_x_min,search_x_max);

  //GenHist->GetYaxis()->SetTickLength(0.015);
  //GenHist->GetXaxis()->SetTickLength(0.02);

  //gPad->SetGridx(1);
  TExec *ex1 = new TExec("ex1","gStyle->SetErrorX(0);");
  TExec *ex2 = new TExec("ex2","gStyle->SetErrorX(0.5);");

  GenHist->SetTitle("");
  GenHist->SetMarkerStyle(20);
  GenHist->SetMarkerSize(1.3);
  GenHist->SetLineColor(1);
  GenHist->GetXaxis()->SetTitle(xtitlename);
  GenHist->GetYaxis()->SetTitle(ytitlename);
  for (int i=1;i<=6;i++) {
    GenHist->GetXaxis()->SetBinLabel(i,cut_labels[i-1].Data());
  }
  GenHist->Scale(lumi/lumi_ref);
  EstHist->Scale(lumi/lumi_ref);
  TH1D * GenHist_Normalize = static_cast<TH1D*>(GenHist->Clone("GenHist_Normalize"));
  GenHist_Normalize->SetMaximum(ymax_top);
  GenHist_Normalize->SetMinimum(ymin_top);
  ex1->Draw();
  //GenHist_Normalize->GetListOfFunctions()->Add(ex1);
  GenHist_Normalize->DrawCopy("e");

  EstHist->SetFillStyle(3144);
  EstHist->SetFillColor(kRed-10);
  EstHist->SetMarkerStyle(20);
  EstHist->SetMarkerSize(0.0001);
  TH1D * EstHist_Normalize = static_cast<TH1D*>(EstHist->Clone("EstHist_Normalize"));
  ex2->Draw();
  //EstHist_Normalize->GetListOfFunctions()->Add(ex2);
  EstHist_Normalize->DrawCopy("e2same");
  EstHist_Normalize->DrawCopy("esame");

  GenHist->Print("all");
  EstHist->Print("all");

  // dashed vertical line to separate into two NJets regions
  TLine *tl_njet = new TLine();
  tl_njet->SetLineStyle(2);
  tl_njet->DrawLine( 3.5,ymin_top, 3.5,ymax_top);

  // Njet labels
  TLatex * ttext_njet = new TLatex();
  ttext_njet->SetTextFont(42);
  ttext_njet->SetTextSize(0.05);
  ttext_njet->SetTextAlign(22);
  ttext_njet->DrawLatex(1.1, ymax_top/40. , "7 #leq N_{jets} #leq 8");
  ttext_njet->DrawLatex(3.9, ymax_top/40. , "N_{jets} #geq 9");

  //
  // Re-draw to have "expectation" on top of "prediction"
  ex1->Draw();
  GenHist_Normalize->DrawCopy("esame");
  //

  TString line = "";
  sprintf(tempname,"%8.1f",lumi);
  line+=tempname;
  line+=" fb^{-1} (13 TeV)";
  
  int iPeriod = 0;    // 1=7TeV, 2=8TeV, 3=7+8TeV, 7=7+8+13TeV, 0=free form (uses lumi_sqrtS)
  int iPos=0;
    
  writeExtraText = true;  
  if(doDataVsMC) extraText   = "        Preliminary";
  else extraText   = "        Simulation";
  //float extraTextFont = 52;  // default is helvetica-italics

  // text sizes and text offsets with respect to the top frame
  // in unit of the top margin size
  //lumiTextSize     = 0.5;
  //float lumiTextOffset   = 0.2;
  //cmsTextSize      = 0.65;
  //float cmsTextOffset    = 0.1;  // only used in outOfFrame version
  
  //relPosX    = 0.045;
  //relPosY    = 0.035;
  //relExtraDY = 1.2;
  
  // ratio of "CMS" and extra text size
  //float extraOverCmsTextSize  = 0.76;
    
  //TString lumi_13TeV = "20.1 fb^{-1}";
  //TString lumi_8TeV  = "19.7 fb^{-1}";
  //TString lumi_7TeV  = "5.1 fb^{-1}";
  TString lumi_sqrtS = line;


  // Legend & texts
  sprintf(tempname,"Lost-lepton background");
  catLeg1->SetHeader(tempname);
  //sprintf(tempname,"#tau_{hadronic} BG expectation (MC truth)");
  sprintf(tempname,"Direct from simulation");
  catLeg1->AddEntry(GenHist,tempname,"p");
  //sprintf(tempname,"Prediction from MC");
  if(doDataVsMC) sprintf(tempname,"Prediction from data");
  else sprintf(tempname,"Treat simulation like data");
  catLeg1->AddEntry(EstHist,tempname);
  catLeg1->Draw();

  gPad->RedrawAxis();

  //
  // Bottom ratio plot
  //
  // ----------

    //
    // Preparing ratio histograms
      TH1D * numerator   = static_cast<TH1D*>(GenHist->Clone("numerator"));
      TH1D * numerator_fullstaterr   = static_cast<TH1D*>(GenHist->Clone("numerator_fullstaterr"));
      TH1D * denominator = static_cast<TH1D*>(EstHist->Clone("denominator"));

      TH1D * GenHist_Clone = static_cast<TH1D*>(GenHist->Clone("GenHist_Clone"));
      TH1D * EstHist_Clone = static_cast<TH1D*>(EstHist->Clone("EstHist_Clone"));
      TH1D * EstHist_NoError = static_cast<TH1D*>(EstHist->Clone("EstHist_NoError"));
      TH1D * One_NoError = static_cast<TH1D*>(EstHist->Clone("EstHist_NoError"));
      for (int ibin=0; ibin<EstHist_NoError->GetNbinsX()+2; ibin++){ // scan including underflow and overflow bins
	EstHist_NoError->SetBinError(ibin,0.);
	One_NoError->SetBinContent(ibin,1.);
	One_NoError->SetBinError(ibin,0.);
      }

      //EstHistD->Add(GenHistD,-1);
      numerator->Divide(GenHist_Clone,EstHist_NoError,1,1,"");
      denominator->Divide(EstHist_Clone,EstHist_NoError,1,1,"");

      numerator_fullstaterr->Divide(GenHist_Clone,EstHist_Clone,1,1,"");  // Expectation/Prediction
      numerator_fullstaterr->Add(One_NoError,-1.);                        // Expectation/Prediction-1

      // draw bottom figure
      canvas_dw->cd();
      // font size
      numerator->GetXaxis()->SetLabelSize(0.175);
      numerator->GetXaxis()->SetTitleSize(font_size_dw);
      numerator->GetYaxis()->SetLabelSize(font_size_dw);
      numerator->GetYaxis()->SetTitleSize(font_size_dw);

      //
      // Horizontal Lines
      TLine *tline  = new TLine(search_x_min,1.,search_x_max,1.);
      TLine *tline0 = new TLine(search_x_min,0.,search_x_max,0.);

      //
      // Common to all bottom plots
      //
      //sprintf(ytitlename,"#frac{Estimate - #tau_{had} BG}{#tau_{had} BG} ");
      sprintf(ytitlename,"#frac{Direct}{Prediction} ");
      numerator->SetMaximum(1.5);
      numerator->SetMinimum(0.5);

      //
      // Specific to each bottom plot
      //
      // Setting style
      //numerator->SetMaximum(1.4);
      //numerator->GetXaxis()->SetLabelFont(42);
      //numerator->GetXaxis()->SetLabelOffset(0.007);
      numerator->GetXaxis()->SetLabelSize(0.18*0.045/0.06);
      numerator->GetXaxis()->SetTitleSize(0.18);
      numerator->GetXaxis()->SetTitleOffset(0.9);
      numerator->GetXaxis()->SetTitleFont(42);
      //numerator->GetYaxis()->SetLabelFont(42);
      //numerator->GetYaxis()->SetLabelOffset(0.007);
      numerator->GetYaxis()->SetLabelSize(0.18*0.045/0.06);
      numerator->GetYaxis()->SetTitleSize(0.18);
      //numerator->GetYaxis()->SetTitleOffset(0.5);
      numerator->GetYaxis()->SetTitleOffset(0.25);
      numerator->GetYaxis()->SetTitleFont(42);

      numerator->GetXaxis()->SetTitle(xtitlename);
      numerator->GetYaxis()->SetTitle(ytitlename);

      //gPad->SetGridx(1);


      if (pull==1){

	sprintf(ytitlename,"#frac{Exp - Pre}{Stat Error} ");
	numerator->SetMaximum(8.);
	numerator->SetMinimum(-8.);
	
	//
	// Specific to each bottom plot
	//
	// Setting style

	for (int ibin=0; ibin<numerator_fullstaterr->GetNbinsX()+2; ibin++){ // scan including underflow and overflow bins
	  numerator_fullstaterr->SetBinContent(ibin,numerator_fullstaterr->GetBinContent(ibin)/numerator_fullstaterr->GetBinError(ibin));
	  numerator_fullstaterr->SetBinError(ibin,0.);
	}
	numerator_fullstaterr->Print("all");
	
	numerator_fullstaterr->GetXaxis()->SetLabelSize(font_size_dw);
	numerator_fullstaterr->GetXaxis()->SetTitleSize(font_size_dw);
	numerator_fullstaterr->GetYaxis()->SetLabelSize(font_size_dw);
	numerator_fullstaterr->GetYaxis()->SetTitleSize(font_size_dw);

	numerator_fullstaterr->GetXaxis()->SetTitleSize(0.12);
	numerator_fullstaterr->GetXaxis()->SetTitleOffset(0.9);
	numerator_fullstaterr->GetXaxis()->SetTitleFont(42);
	numerator_fullstaterr->GetYaxis()->SetTitleSize(0.13);
	numerator_fullstaterr->GetYaxis()->SetTitleOffset(0.5);
	numerator_fullstaterr->GetYaxis()->SetTitleFont(42);
	
	numerator_fullstaterr->GetXaxis()->SetTitle(xtitlename);
	numerator_fullstaterr->GetYaxis()->SetTitle(ytitlename);
	//numerator_fullstaterr->SetFillColor(kGreen-3);
	numerator_fullstaterr->SetFillColor(kRed-10);
	numerator_fullstaterr->DrawCopy();

	//
	// Drawing lines
	tline0->SetLineStyle(2);
	//tline0->Draw();

      }
      else {

      //
      // Plotting
      numerator->GetYaxis()->SetNdivisions(505);
      numerator->GetYaxis()->SetTickLength(0.015);
      numerator->GetXaxis()->SetTickLength(0.08);
      numerator->SetTitle("");
      ex1->Draw();
      numerator->DrawCopy();

      ex2->Draw();
      denominator->DrawCopy("e2same");
      denominator->DrawCopy("same");

      ex1->Draw();
      numerator->DrawCopy("same");

      numerator->Print("all");
      denominator->Print("all");
      numerator_fullstaterr->Print("all");

      //
      // Drawing lines
      tline->SetLineStyle(2);
      //tline->Draw();

      }
      
      // Njet separation lines

      tl_njet->DrawLine( 3.5,ymin_bottom, 3.5,ymax_bottom); 


      gPad->RedrawAxis();

      //
      //

  CMS_lumi(canvas, iPeriod, iPos, lumi_sqrtS);

  if(doDataVsMC){
    sprintf(tempname,"DataMC_Full_Plot.pdf");
    if (pull==1)    sprintf(tempname,"DataMCPull_Full_Plot.pdf");
  }else{
    if(option.find("QCD")!=string::npos) sprintf(tempname,"Closure_QCD_HDP_Full_Plot.pdf");
      else sprintf(tempname,"Closure_Full_Plot.pdf");
    if (pull==1)    sprintf(tempname,"ClosurePull_Full_Plot.pdf");
  }

  canvas->Print(tempname);

}

