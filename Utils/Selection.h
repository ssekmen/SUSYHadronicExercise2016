#ifndef SELECTION_H
#define SELECTION_H

// Event selection helper functions and cut values.
//
// Author: Matthias Schroeder
//         matthias.schroeder@AT@desy.de
//         November 2013
class Selection {
public:
  // Returns result of delta-phi selection
  static bool deltaPhi(double dPhi1, double dPhi2, double dPhi3) {
    return dPhi1 > 0.5 && dPhi2 > 0.5 && dPhi3 > 0.3;
  }

  // Returns result of HT baseline selection
  static bool ht(double ht) { return ht > 500.; }

  // Returns result of MHT baseline selection
  static bool mht(double mht) { return mht > 200.; }

  // Returns result of N(jets) baseline selection
  static bool nJets(unsigned int nJets) { return nJets >= 3; }

  // Returns result of search-bin selection (to be applied on top
  // of baseline selection)
  static unsigned int searchBin(double ht, double mht, unsigned int nJets);

  // Cut values for HT jets
  static float htJetPtMin() { return 50.; }
  static float htJetEtaMax() { return 2.5; }

  // Cut values for MHT jets
  static float mhtJetPtMin() { return 30.; }
  static float mhtJetEtaMax() { return 5.0; }

};


unsigned int Selection::searchBin(double ht, double mht, unsigned int nJets) {
  unsigned int bin = 0;

  if( nJets >= 8 ) {
    
    if( ht > 1000 ) {

      if(      mht > 200 && mht < 400 ) bin = 1;
      else if( mht > 400 && mht < 600 ) bin = 2;
      else if( mht > 600 && mht < 800 ) bin = 3;
      else if( mht > 800              ) bin = 4;

    }

  }    
  return bin;
}
#endif

