// Print the expected event yields after the event selection
// for the background samples.
//
// Author: Matthias Schroeder
//         matthias.schroeder@AT@desy.de
//         November 2013

#include "../Utils/Sample.h"
#include "../Utils/StyleMatters.h"
#include "../Utils/HistReader.h"

// === Main Function ===================================================
void plotYields_signal() {

  StyleMatters::init();

  const int kNSamples = 4;
  unsigned int ids[kNSamples] = { 31, 32, 33, 34 };

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
    std::string str = Sample::toTString(ids[s]).Data();
    printf(" %-20s:",str.c_str());
  }
  std::cout << std::endl;
  for(int bin = 1; bin <= hYields[0]->GetNbinsX(); ++bin) {
    std::cout << hYields[0]->GetXaxis()->GetBinLabel(bin) << " : \t" << std::flush;
    for(int s = 0; s < kNSamples; ++s) {
      printf("%8.3f +/- ",hYields[s]->GetBinContent(bin));
      printf("%7.3f :",hYields[s]->GetBinError(bin));
    }
    std::cout << std::endl;
  }
}
