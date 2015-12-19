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

  // Ntuple file name of a small subset of the full sample
  // for quick tests
  static TString fileNameSubSample(unsigned int id);

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


//TString Sample::path_ = "root://cmseos:1094//eos/uscms/store/user/lpcsusyhad/PHYS14_720_Dec23_2014/";
//TString Sample::path_ = "root://cmseos:1094//eos/uscms/store/user/lpcsusyhad/CMSDAS2015/";
//TString Sample::path_ = "root://cmsxrootd-site.fnal.gov//store/user/lpcsusyhad/PHYS14_720_Dec23_2014/";
TString Sample::path_ = "root://cmseos:1094//eos/uscms/store/user/lpcsusyhad/SusyRA2Analysis2015/";

std::vector<TString> Sample::fileNameFullSample(unsigned int id, std::vector<double> &xSecVec, std::vector<int> &nEvtVec) {
  xSecVec.clear(); nEvtVec.clear();
  checkId(id);

  std::vector<TString> nameVec;
  TString name("");
//  if(      id ==  1  ) name += "HTMHT-Run2012.root";
//  else if( id ==  2  ) name += "MuHad.root";

  if( id == 11  ){
    //name = path_+"PU20bx25_WJetsToLNu_HT-200to400_madgraph-tauola/*.root"; nameVec.push_back(name); xSecVec.push_back(471.6); nEvtVec.push_back(4686783);
    //name = path_+"PU20bx25_WJetsToLNu_HT-400to600_madgraph-tauola/*.root"; nameVec.push_back(name); xSecVec.push_back(55.61); nEvtVec.push_back(4640594);
    //name = path_+"PU20bx25_WJetsToLNu_HT-600toInf_madgraph-tauola/*.root"; nameVec.push_back(name); xSecVec.push_back(18.81); nEvtVec.push_back(4581841);
    name = path_+"Run2ProductionV5/*WJetsToLNu_HT-100To200*.root"; nameVec.push_back(name); xSecVec.push_back(1.); nEvtVec.push_back(1.);
    name = path_+"Run2ProductionV5/*WJetsToLNu_HT-200To400*.root"; nameVec.push_back(name); xSecVec.push_back(1.); nEvtVec.push_back(1.);
    name = path_+"Run2ProductionV5/*WJetsToLNu_HT-400To600*.root"; nameVec.push_back(name); xSecVec.push_back(1.); nEvtVec.push_back(1.);
    name = path_+"Run2ProductionV5/*WJetsToLNu_HT-600To800*.root"; nameVec.push_back(name); xSecVec.push_back(1.); nEvtVec.push_back(1.);
    name = path_+"Run2ProductionV5/*WJetsToLNu_HT-800To1200*.root"; nameVec.push_back(name); xSecVec.push_back(1.); nEvtVec.push_back(1.);
    name = path_+"Run2ProductionV5/*WJetsToLNu_HT-2500ToInf*.root"; nameVec.push_back(name); xSecVec.push_back(1.); nEvtVec.push_back(1.);
  } else if( id == 12  ){
    //name = path_+"Skims/Run2ProductionV5/tree_signal/tree_TTJets_SingleLeptFromT.root"; nameVec.push_back(name); xSecVec.push_back(1.); nEvtVec.push_back(1.);
    //name = path_+"Skims/Run2ProductionV5/tree_signal/tree_TTJets_SingleLeptFromTbar.root"; nameVec.push_back(name); xSecVec.push_back(1.); nEvtVec.push_back(1.);
    //name = path_+"Run2ProductionV5/*TTJets*SingleLeptFromTbar*.root"; nameVec.push_back(name); xSecVec.push_back(1.); nEvtVec.push_back(1.);
    name = path_+"Run2ProductionV5/*TTJets_TuneCUETP8M1*.root"; nameVec.push_back(name); xSecVec.push_back(1.); nEvtVec.push_back(1.);
  } else if( id == 13 ){
    //name = path_+"PU20bx25_ZJetsToNuNu_HT-200to400-madgraph-tauola/*.root"; nameVec.push_back(name); xSecVec.push_back(100.8); nEvtVec.push_back(4546470);
    //name = path_+"PU20bx25_ZJetsToNuNu_HT-400to600-madgraph-tauola/*.root"; nameVec.push_back(name); xSecVec.push_back(11.99); nEvtVec.push_back(4433784);
    //name = path_+"PU20bx25_ZJetsToNuNu_HT-600toInf-madgraph-tauola/*.root"; nameVec.push_back(name); xSecVec.push_back(4.113); nEvtVec.push_back(4463806);
    name = path_+"Run2ProductionV5/*ZJetsToNuNu_HT-100To200*.root"; nameVec.push_back(name); xSecVec.push_back(1.); nEvtVec.push_back(1.);
    name = path_+"Run2ProductionV5/*ZJetsToNuNu_HT-200To400*.root"; nameVec.push_back(name); xSecVec.push_back(1.); nEvtVec.push_back(1.);
    name = path_+"Run2ProductionV5/*ZJetsToNuNu_HT-400To600*.root"; nameVec.push_back(name); xSecVec.push_back(1.); nEvtVec.push_back(1.);
    name = path_+"Run2ProductionV5/*ZJetsToNuNu_HT-600ToInf*.root"; nameVec.push_back(name); xSecVec.push_back(1.); nEvtVec.push_back(1.);
  } else if( id == 14 ){
    //name = path_+"PU20bx25_QCD_HT_250to500_13TeV-madgraph/*.root"; nameVec.push_back(name); xSecVec.push_back(670500); nEvtVec.push_back(2004219+663953);
    //name = path_+"PU20bx25_QCD_HT_500to1000_13TeV-madgraph/*.root"; nameVec.push_back(name); xSecVec.push_back(26740); nEvtVec.push_back(3214312+849033);
    //name = path_+"PU20bx25_QCD_HT_1000toInf_13TeV-madgraph/*.root"; nameVec.push_back(name); xSecVec.push_back(769.7); nEvtVec.push_back(1130720+333733);
    name = path_+"Run2ProductionV5/*QCD_HT200to300*.root"; nameVec.push_back(name); xSecVec.push_back(1.); nEvtVec.push_back(1.);
    name = path_+"Run2ProductionV5/*QCD_HT300to500*.root"; nameVec.push_back(name); xSecVec.push_back(1.); nEvtVec.push_back(1.);
    name = path_+"Run2ProductionV5/*QCD_HT700to1000*.root"; nameVec.push_back(name); xSecVec.push_back(1.); nEvtVec.push_back(1.);
    name = path_+"Run2ProductionV5/*QCD_HT700to1000*.root"; nameVec.push_back(name); xSecVec.push_back(1.); nEvtVec.push_back(1.);
    name = path_+"Run2ProductionV5/*QCD_HT1000to1500*.root"; nameVec.push_back(name); xSecVec.push_back(1.); nEvtVec.push_back(1.);
    name = path_+"Run2ProductionV5/*QCD_HT1500to2000*.root"; nameVec.push_back(name); xSecVec.push_back(1.); nEvtVec.push_back(1.);
    name = path_+"Run2ProductionV5/*QCD_HT2000toInf*.root"; nameVec.push_back(name); xSecVec.push_back(1.); nEvtVec.push_back(1.);
  } else if( id == 21  ){
    //name = path_+"PU20bx25_T1tttt_mGl-1500_mLSP-100-madgraph-tauola/slimmed_stopFlatNtuples_1.root"; nameVec.push_back(name); xSecVec.push_back(0.0141903); nEvtVec.push_back(105679);
    name = path_+"Run2ProductionV5/*T1tttt_mGluino-1500_mLSP-100*.root"; nameVec.push_back(name); xSecVec.push_back(1.); nEvtVec.push_back(1.);
  } else if( id == 22  ){
    //name = path_+"PU20bx25_T1tttt_mGl-1200_mLSP-800-madgraph-tauola/slimmed_stopFlatNtuples_1.root"; nameVec.push_back(name); xSecVec.push_back(0.0856418); nEvtVec.push_back(100322);
    name = path_+"Run2ProductionV5/*T1tttt_mGluino-1200_mLSP-800*.root"; nameVec.push_back(name); xSecVec.push_back(1.); nEvtVec.push_back(1.);
  }

  return nameVec;
}


TString Sample::fileNameSubSample(unsigned int id) {
  checkId(id);

  TString name("");
  if(      id ==  1  ) name += "HTMHT-Run2012.root";
  else if( id ==  2  ) name += "MuHad.root";
  else if( id == 11  ) name += "Summer12-WJetsHT400toInf_1.root";
  else if( id == 12  ) name += "Summer12-TTJets-SemiLep_1.root";
  else if( id == 13  ) name += "Summer12-ZJetsHT400toInf.root";
  else if( id == 14  ) name += "Summer12-QCDHT250toInf.root";
  else if( id == 21  ) name += "Summer12-SUSY_LM6.root";
  else if( id == 22  ) name += "Summer12-SUSY_LM9.root";

  return path_+name;
}


// Return the label for a given sample
TString Sample::label(unsigned int id) {
  checkId(id);

  TString label("");
  if(      id ==  1  ) label += "Data";
  else if( id ==  2  ) label += "Data";
  else if( id == 11  ) label += "W(l#nu)+Jets";
  else if( id == 12  ) label += "t#bar{t}+Jets";
  else if( id == 13  ) label += "Z(#nu#bar{#nu})+Jets";
  else if( id == 14  ) label += "QCD";
  else if( id == 21  ) label += "T1tttt(1500,100)";
  else if( id == 22  ) label += "T1tttt(1200,800)";

  return label;
}


TString Sample::toTString(unsigned int id) {
  checkId(id);

  TString str("");
  if(      id ==  1  ) str += "Data";
  else if( id ==  2  ) str += "Data";
  else if( id == 11  ) str += "WJets";
  else if( id == 12  ) str += "TTJets";
  else if( id == 13  ) str += "ZJets";
  else if( id == 14  ) str += "QCD";
  else if( id == 21  ) str += "T1tttt_1500_100";
  else if( id == 22  ) str += "T1tttt_1200_800";

  return str;
}


int Sample::color(unsigned int id) {
  checkId(id);

  int color = kBlack;
  if(      id ==  1  ) color = kBlack;
  else if( id ==  1  ) color = kBlack;
  else if( id == 11  ) color = kGreen+1;
  else if( id == 12  ) color = kRed;
  else if( id == 13  ) color = kYellow+2;
  else if( id == 14  ) color = kRed+3;
  else if( id == 21  ) color = kBlue;
  else if( id == 22  ) color = kBlue+3;

  return color;
}
  

void Sample::checkId(unsigned int id) {
  if( id != 1 && id != 2 && !(id >= 11 && id <= 16) && !(id >= 21 && id <= 22) ) {
    std::cerr << "\n\nERROR invalid sample id " << id << std::endl;
    throw std::exception();
  } 
}

#endif
