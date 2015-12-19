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
  static bool deltaPhi(double dPhi1, double dPhi2, double dPhi3, double dPhi4) {
    return dPhi1 > 0.5 && dPhi2 > 0.5 && dPhi3 > 0.3 && dPhi4 > 0.3;
  }

  // Returns result of HT baseline selection
  static bool ht(double ht) { return ht > 500.; }

  // Returns result of MHT baseline selection
  static bool mht(double mht) { return mht > 200.; }

  // Returns result of N(jets) baseline selection
  static bool nJets(unsigned int nJets) { return nJets >= 4; }

  // Returns result of search-bin selection (to be applied on top
  // of baseline selection)
  static unsigned int searchBin(double ht, double mht, unsigned int nJets, int nBtags);

  // Cut values for HT jets
  static float htJetPtMin() { return 30.; }
  static float htJetEtaMax() { return 2.4; }

  // Cut values for MHT jets
  static float mhtJetPtMin() { return 30.; }
  static float mhtJetEtaMax() { return 5.0; }

};


unsigned int Selection::searchBin(double ht, double mht, unsigned int nJets, int nBtags) {
  unsigned int bin = 0;

  //
  //if (nBtags>=2){

  if( nJets >= 7 && nJets <=8 ) {
    
    if( ht > 800 ) {

      if(      mht > 200 && mht < 500 ) bin = 1;
      else if( mht > 500 && mht < 750 ) bin = 2;
      else if( mht > 750              ) bin = 3;

    }

  }    
  if( nJets >= 9 ) {
    
    if( ht > 800 ) {

      if(      mht > 200 && mht < 500 ) bin = 4;
      else if( mht > 500 && mht < 750 ) bin = 5;
      else if( mht > 750              ) bin = 6;

    }

  }
  //}
  //

  return bin;
}
#endif

