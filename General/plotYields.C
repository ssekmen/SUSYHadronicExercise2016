// Print the expected event yields after the event selection
// for the background samples.
//
// Author: Matthias Schroeder
//         matthias.schroeder@AT@desy.de
//         November 2013


// === Main Function ===================================================
void plotYields() {
  gROOT->ProcessLine(".L ../Utils/Sample.h++");
  gROOT->ProcessLine(".L ../Utils/StyleMatters.h++");
  gROOT->ProcessLine(".L ../Utils/HistReader.h++");
  StyleMatters::init();



  const int kNSamples = 6;
  unsigned int ids[kNSamples] = { 11, 12, 13, 14, 21, 22 };

  TH1* hYields[kNSamples];
  for(int s = 0; s < kNSamples; ++s) { // Loop over samples
    // Get histograms from file
    const TString fileName = "General_"+Sample::toTString(ids[s])+"-Yields.root";
    hYields[s] = HistReader::get(fileName,"hYields");
  } // End of loop over samples


  // Print event yields
  std::cout << "Event yields" << std::endl;
  std::cout << "Selection\t" << std::flush;
  for(int s = 0; s < kNSamples; ++s) {
    std::cout << Sample::toTString(ids[s]) << " \t\t\t" << std::flush;
  }
  std::cout << std::endl;
  for(int bin = 1; bin <= hYields[0]->GetNbinsX(); ++bin) {
    std::cout << hYields[0]->GetXaxis()->GetBinLabel(bin) << " : \t" << std::flush;
    for(int s = 0; s < kNSamples; ++s) {
      std::cout << hYields[s]->GetBinContent(bin) << " +/-" << std::flush;
      std::cout << hYields[s]->GetBinError(bin) << "  \t" << std::flush;
    }
    std::cout << std::endl;
  }
}
