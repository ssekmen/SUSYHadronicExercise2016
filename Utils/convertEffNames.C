{
  gROOT->ProcessLine(".L ../Utils/HistReader.h+");
  gROOT->ProcessLine(".L ../Utils/LeptonEfficiency.h+");
  gROOT->ProcessLine(".L ../Utils/LeptonAcceptance.h+");

  const TString fileName = "~/DAS/MCEff.root";

  TH1* hAccMu  = HistReader::get(fileName,"Muon/MuonAccEff3");
  TH1* hIsoMu0 = HistReader::get(fileName,"Muon/IsoHTMHTMuEffNJet_35");
  TH1* hIsoMu1 = HistReader::get(fileName,"Muon/IsoHTMHTMuEffNJet_67");
  TH1* hIsoMu2 = HistReader::get(fileName,"Muon/IsoHTMHTMuEffNJet_8Inf");
  TH1* hRecMu0 = HistReader::get(fileName,"Muon/RecoHTMHTMuEffNJet_35");
  TH1* hRecMu1 = HistReader::get(fileName,"Muon/RecoHTMHTMuEffNJet_67");
  TH1* hRecMu2 = HistReader::get(fileName,"Muon/RecoHTMHTMuEffNJet_8Inf");
  
  TH1* hAccEl  = HistReader::get(fileName,"Electron/ElecAccEff3");
  TH1* hIsoEl0 = HistReader::get(fileName,"Electron/IsoHTMHTElecEffNJet_35");
  TH1* hIsoEl1 = HistReader::get(fileName,"Electron/IsoHTMHTElecEffNJet_67");
  TH1* hIsoEl2 = HistReader::get(fileName,"Electron/IsoHTMHTElecEffNJet_8Inf");
  TH1* hRecEl0 = HistReader::get(fileName,"Electron/RecoHTMHTElecEffNJet_35");
  TH1* hRecEl1 = HistReader::get(fileName,"Electron/RecoHTMHTElecEffNJet_67");
  TH1* hRecEl2 = HistReader::get(fileName,"Electron/RecoHTMHTElecEffNJet_8Inf");

  hAccMu->SetName(LeptonAcceptance::nameMuonAcc()); 
  hIsoMu0->SetName(LeptonEfficiency::nameMuonRecoEff(0)); 
  hIsoMu1->SetName(LeptonEfficiency::nameMuonRecoEff(1)); 
  hIsoMu2->SetName(LeptonEfficiency::nameMuonRecoEff(2)); 
  hRecMu0->SetName(LeptonEfficiency::nameMuonIsoEff(0)); 
  hRecMu1->SetName(LeptonEfficiency::nameMuonIsoEff(1)); 
  hRecMu2->SetName(LeptonEfficiency::nameMuonIsoEff(2)); 
  
  hAccEl->SetName(LeptonAcceptance::nameElecAcc()); 
  hIsoEl0->SetName(LeptonEfficiency::nameElecRecoEff(0)); 
  hIsoEl1->SetName(LeptonEfficiency::nameElecRecoEff(1)); 
  hIsoEl2->SetName(LeptonEfficiency::nameElecRecoEff(2)); 
  hRecEl0->SetName(LeptonEfficiency::nameElecIsoEff(0)); 
  hRecEl1->SetName(LeptonEfficiency::nameElecIsoEff(1)); 
  hRecEl2->SetName(LeptonEfficiency::nameElecIsoEff(2)); 

  TFile* file = new TFile("LLFullEff.root","RECREATE");
  file->WriteTObject(hAccMu); 
  file->WriteTObject(hIsoMu0); 
  file->WriteTObject(hIsoMu1); 
  file->WriteTObject(hIsoMu2); 
  file->WriteTObject(hRecMu0);
  file->WriteTObject(hRecMu1);
  file->WriteTObject(hRecMu2);
  
  file->WriteTObject(hAccEl); 
  file->WriteTObject(hIsoEl0); 
  file->WriteTObject(hIsoEl1); 
  file->WriteTObject(hIsoEl2); 
  file->WriteTObject(hRecEl0);
  file->WriteTObject(hRecEl1);
  file->WriteTObject(hRecEl2);
  
  delete file;
}
