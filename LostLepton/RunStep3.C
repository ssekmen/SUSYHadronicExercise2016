// Step 3: calculate the weights needed to extrapolate from control sample to search region using the efficiencies measured in Step 2

#ifdef __CINT__
#pragma link C++ class std::vector<TLorentzVector>+;
#endif

#include "PredictionMaker.C"

void RunStep3() {

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
  PredictionMaker* pred_mkr = new PredictionMaker(mc);
  pred_mkr->Run("Efficiencies.root", "Prediction_MC1.root");

  delete pred_mkr;

  // treat the inclusive samples separately--must cut on genHT
  TChain* inc_mc = new TChain("tree");
  inc_mc->Add("root://cmseos.fnal.gov///store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/CMSDAS2016/tree_SL/tree_TTJets_DiLept.root");
  inc_mc->Add("root://cmseos.fnal.gov///store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/CMSDAS2016/tree_SL/tree_TTJets_SingleLeptFromTbar.root");
  inc_mc->Add("root://cmseos.fnal.gov///store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/CMSDAS2016/tree_SL/tree_TTJets_SingleLeptFromT.root");

  PredictionMaker* inc_pred_mkr = new PredictionMaker(inc_mc);
  inc_pred_mkr->Run("Efficiencies.root", "Prediction_MC2.root",600);

  delete inc_pred_mkr;

  // now use hadd to combine the two MC output files into one:
  // hadd Prediction.root Prediction_MC1.root Prediction_MC2.root; rm Prediction_MC*root;
  

  // run the prediction on data separately
  // need to change runOnData to true in PredictionMaker.h,
  // then uncomment these lines to run on data
  // TChain* data = new TChain("tree");
  // data->Add("root://cmseos.fnal.gov///store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/CMSDAS2016/tree_SL/tree_HTMHT_2015Db.root");
  // data->Add("root://cmseos.fnal.gov///store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/CMSDAS2016/tree_SL/tree_HTMHT_re2015D.root");
  // data->Add("root://cmseos.fnal.gov///store/user/lpcsusyhad/SusyRA2Analysis2015/Skims/CMSDAS2016/tree_SL/tree_HTMHT_re2015C.root");

  // PredictionMaker* data_pred_mkr = new PredictionMaker(data);
  // data_pred_mkr->Run("Efficiencies.root", "Prediction_data.root");

  // delete data_pred_mkr;

}
