#ifndef SAMPLE_H
#define SAMPLE_H

#include <exception>
#include <iostream>

#include "TColor.h"
#include "TString.h"


// Encapsulates sample information. Each sample is
// accessed by its unique id as defined in the table
// on the TWiki.
//
// Author: Matthias Schroeder
//         matthias.schroeder@AT@desy.de
//         November 2013
class Sample {
public:
  // Compact sample name without spaces, i.e. suitable
  // for file names etc.
  static TString toTString(unsigned int id);

  // Ntuple file name
  static std::vector<TString> fileNameFullSample(unsigned int id, std::vector<double> &xSecVec, std::vector<int> &nEvtVec);

  // Full-blown sample name including TLatex commands for
  // plot legends etc.
  static TString label(unsigned int id);

  // A color associated with each sample, e.g. for plotting
  static int color(unsigned int id);
  
//  static vector<double> getxSec(){ return xSecVec; }

private:
  static TString path_;

  static void checkId(unsigned int id);

//  static vector<double> xSecVec;
};

TString Sample::path_ = "root://cmseos.fnal.gov//store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/CMSDAS2016/";

std::vector<TString> Sample::fileNameFullSample(unsigned int id, std::vector<double> &xSecVec, std::vector<int> &nEvtVec) {
  xSecVec.clear(); nEvtVec.clear();
  checkId(id);

  std::vector<TString> nameVec;
  TString name("");
  if(      id ==  1  ){
    name = path_+"tree_signal/tree_HTMHT_re2015C.root";   nameVec.push_back(name); xSecVec.push_back(1.); nEvtVec.push_back(1.);
    name = path_+"tree_signal/tree_HTMHT_re2015D.root";   nameVec.push_back(name); xSecVec.push_back(1.); nEvtVec.push_back(1.);
    name = path_+"tree_signal/tree_HTMHT_2015Db.root";    nameVec.push_back(name); xSecVec.push_back(1.); nEvtVec.push_back(1.);
  }

  //
  // W+jets
  if( id == 11  ){
    name = path_+"tree_signal/tree_WJetsToLNu_HT-100to200.root";   nameVec.push_back(name); xSecVec.push_back(1627.45); nEvtVec.push_back(10152718);
    name = path_+"tree_signal/tree_WJetsToLNu_HT-200to400.root";   nameVec.push_back(name); xSecVec.push_back( 435.24); nEvtVec.push_back( 5221599);
    name = path_+"tree_signal/tree_WJetsToLNu_HT-400to600.root";   nameVec.push_back(name); xSecVec.push_back(  59.18); nEvtVec.push_back( 1745914);
    name = path_+"tree_signal/tree_WJetsToLNu_HT-600to800.root";   nameVec.push_back(name); xSecVec.push_back(  14.58); nEvtVec.push_back( 4041997);
    name = path_+"tree_signal/tree_WJetsToLNu_HT-800to1200.root";  nameVec.push_back(name); xSecVec.push_back(   6.66); nEvtVec.push_back( 1574633);
    name = path_+"tree_signal/tree_WJetsToLNu_HT-1200to2500.root"; nameVec.push_back(name); xSecVec.push_back(  1.608); nEvtVec.push_back(  255637);
    name = path_+"tree_signal/tree_WJetsToLNu_HT-2500toInf.root";  nameVec.push_back(name); xSecVec.push_back(0.03891); nEvtVec.push_back(  253036);
  //
  // TTbar
  } else if( id == 12  ){
    name = path_+"tree_signal/tree_TTJets_SingleLeptFromT.root";    nameVec.push_back(name); xSecVec.push_back(182.72); nEvtVec.push_back(60186393);
    name = path_+"tree_signal/tree_TTJets_SingleLeptFromTbar.root"; nameVec.push_back(name); xSecVec.push_back(182.72); nEvtVec.push_back(59816364);
    name = path_+"tree_signal/tree_TTJets_DiLept.root";             nameVec.push_back(name); xSecVec.push_back( 88.34); nEvtVec.push_back(30498962);
    //name = path_+"tree_signal/tree_TTJets_HT-600to800.root";   nameVec.push_back(name); xSecVec.push_back(2.678835503); nEvtVec.push_back(5119009);
    //name = path_+"tree_signal/tree_TTJets_HT-800to1200.root";  nameVec.push_back(name); xSecVec.push_back(1.103362965); nEvtVec.push_back(3510897);
    //name = path_+"tree_signal/tree_TTJets_HT-1200to2500.root"; nameVec.push_back(name); xSecVec.push_back(0.199703704); nEvtVec.push_back(1014678);
    //name = path_+"tree_signal/tree_TTJets_HT-2500toInf.root";  nameVec.push_back(name); xSecVec.push_back(0.002379801); nEvtVec.push_back( 507842);
  // 
  // Z(nunu)+jets
  } else if( id == 13 ){
    name = path_+"tree_signal/tree_ZJetsToNuNu_HT-100to200.root"; nameVec.push_back(name); xSecVec.push_back(344.3);  nEvtVec.push_back(5154824);
    name = path_+"tree_signal/tree_ZJetsToNuNu_HT-200to400.root"; nameVec.push_back(name); xSecVec.push_back( 95.23); nEvtVec.push_back(4998316);
    name = path_+"tree_signal/tree_ZJetsToNuNu_HT-400to600.root"; nameVec.push_back(name); xSecVec.push_back( 13.19); nEvtVec.push_back(1018882);
    name = path_+"tree_signal/tree_ZJetsToNuNu_HT-600toInf.root"; nameVec.push_back(name); xSecVec.push_back( 5.063); nEvtVec.push_back(1008333);
  //
  // QCD
  } else if( id == 14 ){
    name = path_+"tree_signal/tree_QCD_HT-300to500.root";   nameVec.push_back(name); xSecVec.push_back(351300.); nEvtVec.push_back(19826197);
    name = path_+"tree_signal/tree_QCD_HT-500to700.root";   nameVec.push_back(name); xSecVec.push_back( 31630.); nEvtVec.push_back(19664159);
    name = path_+"tree_signal/tree_QCD_HT-700to1000.root";  nameVec.push_back(name); xSecVec.push_back(  6802.); nEvtVec.push_back(15356448);
    name = path_+"tree_signal/tree_QCD_HT-1000to1500.root"; nameVec.push_back(name); xSecVec.push_back(  1206.); nEvtVec.push_back(4963895);
    name = path_+"tree_signal/tree_QCD_HT-1500to2000.root"; nameVec.push_back(name); xSecVec.push_back(  120.4); nEvtVec.push_back(3868886);
    name = path_+"tree_signal/tree_QCD_HT-2000toInf.root";  nameVec.push_back(name); xSecVec.push_back(  25.24); nEvtVec.push_back(1912529);
  // 
  // T1tttt signal
  } else if( id == 21  ){
    name = path_+"tree_signal/tree_T1tttt_1500_100.root"; nameVec.push_back(name); xSecVec.push_back(0.0141903); nEvtVec.push_back(103140);
  } else if( id == 22  ){
    name = path_+"tree_signal/tree_T1tttt_1200_800.root"; nameVec.push_back(name); xSecVec.push_back(0.0856418); nEvtVec.push_back(147194);
  //
  // T1tttt signal fastsim scan 
  } else if( id == 31  ){
    name = path_+"tree_signal/tree_T1tttt_1300_100_fast.root"; nameVec.push_back(name); xSecVec.push_back(0.0460525); nEvtVec.push_back(98710);
  } else if( id == 32  ){
    name = path_+"tree_signal/tree_T1tttt_1400_100_fast.root"; nameVec.push_back(name); xSecVec.push_back(0.0252977); nEvtVec.push_back(102774);
  } else if( id == 33  ){
    name = path_+"tree_signal/tree_T1tttt_1500_100_fast.root"; nameVec.push_back(name); xSecVec.push_back(0.0141903); nEvtVec.push_back(105596);
  } else if( id == 34  ){
    name = path_+"tree_signal/tree_T1tttt_1600_100_fast.root"; nameVec.push_back(name); xSecVec.push_back(0.00810078); nEvtVec.push_back(53985); 
// 
  // W+jets individual HT bins
  } else if( id == 111  ){
    name = path_+"tree_signal/tree_WJetsToLNu_HT-100to200.root";   nameVec.push_back(name); xSecVec.push_back(1627.45); nEvtVec.push_back(10152718);
  } else if( id == 112  ){
    name = path_+"tree_signal/tree_WJetsToLNu_HT-200to400.root";   nameVec.push_back(name); xSecVec.push_back( 435.24); nEvtVec.push_back( 5221599);
  } else if( id == 113  ){
    name = path_+"tree_signal/tree_WJetsToLNu_HT-400to600.root";   nameVec.push_back(name); xSecVec.push_back(  59.18); nEvtVec.push_back( 1745914);
  } else if( id == 114  ){
    name = path_+"tree_signal/tree_WJetsToLNu_HT-600to800.root";   nameVec.push_back(name); xSecVec.push_back(  14.58); nEvtVec.push_back( 4041997);
  } else if( id == 115  ){
    name = path_+"tree_signal/tree_WJetsToLNu_HT-800to1200.root";  nameVec.push_back(name); xSecVec.push_back(   6.66); nEvtVec.push_back( 1574633);
  } else if( id == 116  ){
    name = path_+"tree_signal/tree_WJetsToLNu_HT-1200to2500.root"; nameVec.push_back(name); xSecVec.push_back(  1.608); nEvtVec.push_back(  255637);
  } else if( id == 117  ){
    name = path_+"tree_signal/tree_WJetsToLNu_HT-2500toInf.root";  nameVec.push_back(name); xSecVec.push_back(0.03891); nEvtVec.push_back(  253036);
  }

  return nameVec;
}


// Return the label for a given sample
TString Sample::label(unsigned int id) {
  checkId(id);

  TString label("");
  if(      id ==  1  ) label += "Data";
  else if( id == 11  ) label += "W(l#nu)+Jets";
  else if( id == 12  ) label += "t#bar{t}+Jets";
  else if( id == 13  ) label += "Z(#nu#bar{#nu})+Jets";
  else if( id == 14  ) label += "QCD";
  else if( id == 21  ) label += "T1tttt(1500,100)";
  else if( id == 22  ) label += "T1tttt(1200,800)";
  else if( id == 31  ) label += "T1tttt(1200,100) fastsim";
  else if( id == 32  ) label += "T1tttt(1300,100) fastsim";
  else if( id == 33  ) label += "T1tttt(1400,100) fastsim";
  else if( id == 34  ) label += "T1tttt(1500,100) fastsim";
  else if( id == 35  ) label += "T1tttt(1600,100) fastsim";
  else if( id == 36  ) label += "T1tttt(1700,100) fastsim";
  else if( id == 37  ) label += "T1tttt(1800,100) fastsim";
  else if( id == 111 ) label += "W(l#nu)+Jets HT=[100,200] GeV";
  else if( id == 112 ) label += "W(l#nu)+Jets HT=[200,400] GeV";
  else if( id == 113 ) label += "W(l#nu)+Jets HT=[400,600] GeV";
  else if( id == 114 ) label += "W(l#nu)+Jets HT=[600,800] GeV";
  else if( id == 115 ) label += "W(l#nu)+Jets HT=[800,1200] GeV";
  else if( id == 116 ) label += "W(l#nu)+Jets HT=[1200,2500] GeV";
  else if( id == 117 ) label += "W(l#nu)+Jets HT>=2500 GeV";

  return label;
}


TString Sample::toTString(unsigned int id) {
  checkId(id);

  TString str("");
  if(      id ==  1  ) str += "Data";
  else if( id == 11  ) str += "WJets";
  else if( id == 12  ) str += "TTJets";
  else if( id == 13  ) str += "ZJets";
  else if( id == 14  ) str += "QCD";
  else if( id == 21  ) str += "T1tttt_1500_100";
  else if( id == 22  ) str += "T1tttt_1200_800";
  else if( id == 31  ) str += "T1tttt_1200_100_fastsim";
  else if( id == 32  ) str += "T1tttt_1300_100_fastsim";
  else if( id == 33  ) str += "T1tttt_1400_100_fastsim";
  else if( id == 34  ) str += "T1tttt_1500_100_fastsim";
  else if( id == 35  ) str += "T1tttt_1600_100_fastsim";
  else if( id == 36  ) str += "T1tttt_1700_100_fastsim";
  else if( id == 37  ) str += "T1tttt_1800_100_fastsim";
  else if( id == 111  ) str += "WJets_HT-100to200";
  else if( id == 112  ) str += "WJets_HT-200to400";
  else if( id == 113  ) str += "WJets_HT-400to600";
  else if( id == 114  ) str += "WJets_HT-600to800";
  else if( id == 115  ) str += "WJets_HT-800to1200";
  else if( id == 116  ) str += "WJets_HT-1200to2500";
  else if( id == 117  ) str += "WJets_HT-2500toInf";

  return str;
}


int Sample::color(unsigned int id) {
  checkId(id);

  int color = kBlack;
  if(      id ==  1  ) color = kBlack;
  else if( id == 11  ) color = kGreen+1;
  else if( id == 12  ) color = kRed;
  else if( id == 13  ) color = kYellow+2;
  else if( id == 14  ) color = kRed+3;
  else if( id == 21  ) color = kBlue;
  else if( id == 22  ) color = kBlue+3;
  else if( id == 111 ) color = kBlack;
  else if( id == 112 ) color = kGreen+1;
  else if( id == 113 ) color = kRed;
  else if( id == 114 ) color = kYellow+2;
  else if( id == 115 ) color = kRed+3;
  else if( id == 116 ) color = kBlue;
  else if( id == 117 ) color = kMagenta;

  return color;
}
  

void Sample::checkId(unsigned int id) {
  if( id != 1 
      && !(id >=  11 && id <= 14) 
      && !(id >=  21 && id <= 22) 
      && !(id >=  31 && id <= 34) 
      && !(id >= 111 && id <= 117) 
      ) {
    std::cerr << "\n\nERROR invalid sample id " << id << std::endl;
    throw std::exception();
  } 
}

#endif
