enum binning {  k13TeV };


int scrabble(double MHT, double HT, int NJets ,int BJets){

  //cout << "SCRABBLE" << endl;
  

  if( MHT < 200 || HT < 800 || NJets < 7 ) return -1 ;

  int binMHT = -1;
  int binBJet = -1;

  if( MHT > 200 && MHT < 500 ){

    //cout << "low MHT" << endl;
      binMHT = 1;

  }else if( MHT > 500 && MHT < 750 ){

    //cout << "med MHT" << endl;
      binMHT = 2 ; 

  }else if( MHT > 750 ){

      binMHT = 3 ; 

  }else 
    return -1;


  if( BJets >= 0 && BJets <= 1 ){
      //low BJet
      binBJet = 0 ;

      }else if( BJets >=2  ){
      ////high BJet
      binBJet = 1 ;

      }else

        return -1;




  return binBJet*3+binMHT ;



}



int  computeBin(double MHT, double HT, int NJets,int BJets, binning bins = k13TeV){
  
  //cout << "computeBin" << endl;

  if( bins == k13TeV ) return scrabble( MHT , HT , NJets, BJets ) ;  
  else { 
    cout << "computeBin() No binning found!" << endl;
    return -1; 
  }

}
