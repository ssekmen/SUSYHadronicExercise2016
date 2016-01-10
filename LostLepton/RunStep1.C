// Step 1: create a new tree from MC with variables useful for calculating efficiencies (Step 2) and running the closure test (Step 3)

#include "ExpectationMaker.C"

void RunStep1() {

  //gROOT->ProcessLine(".L ExpectationMaker.C++");
  TChain* mc = new TChain("tree");
  mc->Add("root://cmseos.fnal.gov///store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/CMSDAS2016/tree_SL/tree_ST_tW_antitop.root");
  mc->Add("root://cmseos.fnal.gov///store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/CMSDAS2016/tree_SL/tree_ST_tW_top.root");
  mc->Add("root://cmseos.fnal.gov///store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/CMSDAS2016/tree_SL/tree_TTJets_HT-600to800.root");
  mc->Add("root://cmseos.fnal.gov///store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/CMSDAS2016/tree_SL/tree_WJetsToLNu_HT-200to400.root");
  mc->Add("root://cmseos.fnal.gov///store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/CMSDAS2016/tree_SL/tree_TTJets_HT-800to1200.root");
  mc->Add("root://cmseos.fnal.gov///store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/CMSDAS2016/tree_SL/tree_WJetsToLNu_HT-2500toInf.root");
  mc->Add("root://cmseos.fnal.gov///store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/CMSDAS2016/tree_SL/tree_WJetsToLNu_HT-400to600.root");
  mc->Add("root://cmseos.fnal.gov///store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/CMSDAS2016/tree_SL/tree_ST_s-channel.root");
  mc->Add("root://cmseos.fnal.gov///store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/CMSDAS2016/tree_SL/tree_WJetsToLNu_HT-600to800.root");
  mc->Add("root://cmseos.fnal.gov///store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/CMSDAS2016/tree_SL/tree_ST_t-channel_antitop.root");
  mc->Add("root://cmseos.fnal.gov///store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/CMSDAS2016/tree_SL/tree_TTJets_HT-1200to2500.root");  
  mc->Add("root://cmseos.fnal.gov///store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/CMSDAS2016/tree_SL/tree_WJetsToLNu_HT-100to200.root");
  mc->Add("root://cmseos.fnal.gov///store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/CMSDAS2016/tree_SL/tree_WJetsToLNu_HT-800to1200.root");
  mc->Add("root://cmseos.fnal.gov///store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/CMSDAS2016/tree_SL/tree_ST_t-channel_top.root");
  mc->Add("root://cmseos.fnal.gov///store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/CMSDAS2016/tree_SL/tree_TTJets_HT-2500toInf.root");   
  mc->Add("root://cmseos.fnal.gov///store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/CMSDAS2016/tree_SL/tree_WJetsToLNu_HT-1200to2500.root");
 
  ExpectationMaker* exp_mkr = new ExpectationMaker(mc);
  exp_mkr->Run("Expectation_1.root");

  delete exp_mkr;

  // treat the inclusive samples separately--must cut on genHT
  TChain* inc_mc = new TChain("tree");
  inc_mc->Add("root://cmseos.fnal.gov///store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/CMSDAS2016/tree_SL/tree_TTJets_DiLept.root");
  inc_mc->Add("root://cmseos.fnal.gov///store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/CMSDAS2016/tree_SL/tree_TTJets_SingleLeptFromTbar.root");
  inc_mc->Add("root://cmseos.fnal.gov///store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/CMSDAS2016/tree_SL/tree_TTJets_SingleLeptFromT.root");

  ExpectationMaker* inc_exp_mkr = new ExpectationMaker(inc_mc);
  inc_exp_mkr->Run("Expectation_2.root",600);

  delete inc_exp_mkr;

  // now you'll want to combine the two out files using hadd:
  // hadd Expectation.root Expectation_1.root Expectation_2.root; rm Expectation_*root;


}
