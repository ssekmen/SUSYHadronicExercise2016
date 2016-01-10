// Step 2: calculate lepton efficiencies from MC, using output from Step 1, and save them to a root file

#include "EfficiencyMaker.C"

void RunStep2() {

  TChain* effChain = new TChain("mc_exp_tree");
  effChain->Add("Expectation.root");
  EfficiencyMaker* eff_mkr = new EfficiencyMaker(effChain);
  eff_mkr->Run("Efficiencies.root");

  delete eff_mkr;

}
