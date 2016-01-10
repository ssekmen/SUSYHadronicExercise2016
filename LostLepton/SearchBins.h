#include "TVector2.h"
#include <iostream>
#include <vector>

class Bin
{
 public:
  Bin(){}
  Bin(double HTmin, double HTmax, double MHTmin, double MHTmax, int NJetsmin, int NJetsmax, int BTagsmin, int BTagsmax)
    {
      HTmin_=HTmin;
      HTmax_=HTmax;
      MHTmin_=MHTmin;
      MHTmax_=MHTmax;
      NJetsmin_=NJetsmin;
      NJetsmax_=NJetsmax;
      BTagsmin_=BTagsmin;
      BTagsmax_=BTagsmax;
    }
  double HTmin_, HTmax_, MHTmin_, MHTmax_;
  int NJetsmin_, NJetsmax_, BTagsmin_, BTagsmax_;
  ~Bin(){}
 private:
};

class SearchBins
{
 public:
 SearchBins() : SearchBins(false) {}
  SearchBins(bool useQCDbinning);
  unsigned int GetBinNumber(double HT, double MHT, int NJets, int BTags);
  void PrintUsed();
  Bin* GetSearchBin(int i);
  
  ~SearchBins(){}
 protected:
  std::vector<Bin> bins_;
  std::vector<int> usedBin_;
};


SearchBins::SearchBins(bool useDASBinning)
{
  if(!useDASBinning){
    // HTmin,HTmax,MHTmin,MHTmax,NJetsmin,NJetsmax,BTagsmin,BTagsmax
    // NJets 4,6 BTags=0
    // fixed ht Njets btags all MHT bins
    bins_.push_back( Bin(500,800,200,500,4,6,-1,0) );
    bins_.push_back( Bin(800,1200,200,500,4,6,-1,0) );
    bins_.push_back( Bin(1200,99999,200,500,4,6,-1,0) );
	
    bins_.push_back( Bin(500,1200,500,750,4,6,-1,0) );
    bins_.push_back( Bin(1200,99999,500,750,4,6,-1,0) );
	
    bins_.push_back( Bin(800,99999,750,9999,4,6,-1,0) );
	
    // NJets 4,6 BTags=1
    // fixed ht Njets btags all MHT bins
    bins_.push_back( Bin(500,800,200,500,4,6,1,1) );
    bins_.push_back( Bin(800,1200,200,500,4,6,1,1) );
    bins_.push_back( Bin(1200,99999,200,500,4,6,1,1) );
	
    bins_.push_back( Bin(500,1200,500,750,4,6,1,1) );
    bins_.push_back( Bin(1200,99999,500,750,4,6,1,1) );
	
    bins_.push_back( Bin(800,99999,750,9999,4,6,1,1) );

    // NJets 4,6 BTags=2
    // fixed ht Njets btags all MHT bins
    bins_.push_back( Bin(500,800,200,500,4,6,2,2) );
    bins_.push_back( Bin(800,1200,200,500,4,6,2,2) );
    bins_.push_back( Bin(1200,99999,200,500,4,6,2,2) );
  
    bins_.push_back( Bin(500,1200,500,750,4,6,2,2) );
    bins_.push_back( Bin(1200,99999,500,750,4,6,2,2) );
  
    bins_.push_back( Bin(800,99999,750,9999,4,6,2,2) );

    // NJets 4,6 BTags=>3
    // fixed ht Njets btags all MHT bins
    bins_.push_back( Bin(500,800,200,500,4,6,3,9999) );
    bins_.push_back( Bin(800,1200,200,500,4,6,3,9999) );
    bins_.push_back( Bin(1200,99999,200,500,4,6,3,9999) );
  
    bins_.push_back( Bin(500,1200,500,750,4,6,3,9999) );
    bins_.push_back( Bin(1200,99999,500,750,4,6,3,9999) );
  
    bins_.push_back( Bin(800,99999,750,9999,4,6,3,9999) );

    // NJewts 7,8 BTags=0
    bins_.push_back( Bin(500,800,200,500,7,8,-1,0) );
    bins_.push_back( Bin(800,1200,200,500,7,8,-1,0) );
    bins_.push_back( Bin(1200,99999,200,500,7,8,-1,0) );
  
    bins_.push_back( Bin(500,1200,500,750,7,8,-1,0) );
    bins_.push_back( Bin(1200,99999,500,750,7,8,-1,0) );
  
    bins_.push_back( Bin(800,99999,750,9999,7,8,-1,0) );
	
    // NJewts 7,8 BTags=1
    bins_.push_back( Bin(500,800,200,500,7,8,1,1) );
    bins_.push_back( Bin(800,1200,200,500,7,8,1,1) );
    bins_.push_back( Bin(1200,99999,200,500,7,8,1,1) );
	
    bins_.push_back( Bin(500,1200,500,750,7,8,1,1) );
    bins_.push_back( Bin(1200,99999,500,750,7,8,1,1) );
	
    bins_.push_back( Bin(800,99999,750,9999,7,8,1,1) );

    // NJewts 7,8 BTags=2
    bins_.push_back( Bin(500,800,200,500,7,8,2,2) );
    bins_.push_back( Bin(800,1200,200,500,7,8,2,2) );
    bins_.push_back( Bin(1200,99999,200,500,7,8,2,2) );
	
    bins_.push_back( Bin(500,1200,500,750,7,8,2,2) );
    bins_.push_back( Bin(1200,99999,500,750,7,8,2,2) );
	
    bins_.push_back( Bin(800,99999,750,9999,7,8,2,2) );

    // NJewts 7,8 BTags=>3
    bins_.push_back( Bin(500,800,200,500,7,8,3,9999) );
    bins_.push_back( Bin(800,1200,200,500,7,8,3,9999) );
    bins_.push_back( Bin(1200,99999,200,500,7,8,3,9999) );
 
    bins_.push_back( Bin(500,1200,500,750,7,8,3,9999) );
    bins_.push_back( Bin(1200,99999,500,750,7,8,3,9999) );
	  
    bins_.push_back( Bin(800,99999,750,9999,7,8,3,9999) );


    // NJewts 9,9999 BTags=0
    bins_.push_back( Bin(500,800,200,500,9,9999,-1,0) );
    bins_.push_back( Bin(800,1200,200,500,9,9999,-1,0) );
    bins_.push_back( Bin(1200,99999,200,500,9,9999,-1,0) );
  
    bins_.push_back( Bin(500,1200,500,750,9,9999,-1,0) );
    bins_.push_back( Bin(1200,99999,500,750,9,9999,-1,0) );
  
    bins_.push_back( Bin(800,99999,750,9999,9,9999,-1,0) );
	
	
    // NJewts 9,9999 BTags=1
    bins_.push_back( Bin(500,800,200,500,9,9999,1,1) );
    bins_.push_back( Bin(800,1200,200,500,9,9999,1,1) );
    bins_.push_back( Bin(1200,99999,200,500,9,9999,1,1) );
	
    bins_.push_back( Bin(500,1200,500,750,9,9999,1,1) );
    bins_.push_back( Bin(1200,99999,500,750,9,9999,1,1) );
	
    bins_.push_back( Bin(800,99999,750,9999,9,9999,1,1) );

	
    // NJewts 9,9999 BTags=2
    bins_.push_back( Bin(500,800,200,500,9,9999,2,2) );
    bins_.push_back( Bin(800,1200,200,500,9,9999,2,2) );
    bins_.push_back( Bin(1200,99999,200,500,9,9999,2,2) );
	
    bins_.push_back( Bin(500,1200,500,750,9,9999,2,2) );
    bins_.push_back( Bin(1200,99999,500,750,9,9999,2,2) );
	
    bins_.push_back( Bin(800,99999,750,9999,9,9999,2,2) );
	

    // NJewts 9,9999 BTags=>3
    bins_.push_back( Bin(500,800,200,500,9,9999,3,9999) );
    bins_.push_back( Bin(800,1200,200,500,9,9999,3,9999) );
    bins_.push_back( Bin(1200,99999,200,500,9,9999,3,9999) );
	
    bins_.push_back( Bin(500,1200,500,750,9,9999,3,9999) );
    bins_.push_back( Bin(1200,99999,500,750,9,9999,3,9999) );
	
    bins_.push_back( Bin(800,99999,750,9999,9,9999,3,9999) );

  }else{

    bins_.push_back( Bin(500,99999,200,500,7,8,2,2) );
    bins_.push_back( Bin(500,99999,200,500,7,8,3,999) );
    bins_.push_back( Bin(500,99999,500,99999,7,8,2,999) );

    bins_.push_back( Bin(500,99999,200,500,9,99999,2,2) );
    bins_.push_back( Bin(500,99999,200,500,9,99999,3,999) );
    bins_.push_back( Bin(500,99999,500,99999,9,99999,2,999) );


  }

  std::cout<<"Loaded bins: "<<bins_.size()<<std::endl;
  for(unsigned int i=0; i<bins_.size();i++)
    {
      usedBin_.push_back(0); 
    }
}

Bin* SearchBins::GetSearchBin(int i){
  return &(bins_.at(i));
}

unsigned int SearchBins::GetBinNumber(double HT, double MHT, int NJets, int BTags)
{
  unsigned int result =999;
  int match =-1;
  for(unsigned int i=0; i<bins_.size();i++)
    {
      //              std::cout<<"Bin["<<i<<"]: HT["<<bins_[i].HTmin_<<","<<bins_[i].HTmax_<<"] MHT["<<bins_[i].MHTmin_<<","<<bins_[i].MHTmax_<<"] NJets["<<bins_[i].NJetsmin_<<","<<bins_[i].NJetsmax_<<"] BTags["<<bins_[i].BTagsmin_<<","<<bins_[i].BTagsmax_<<"]\n";
      if(HT>=bins_[i].HTmin_ && 
	 HT<bins_[i].HTmax_ &&
	 MHT>=bins_[i].MHTmin_ && 
	 MHT<bins_[i].MHTmax_ &&
	 NJets+0.1>bins_[i].NJetsmin_ && 
	 NJets-0.1<bins_[i].NJetsmax_ &&
	 BTags+0.1>bins_[i].BTagsmin_ && 
	 BTags-0.1<bins_[i].BTagsmax_
	 )
	{
	  result=i;
	  match++;
	  usedBin_[i]=usedBin_[i]+1;
	}
    }
  if(match==-1)
    {
      //      if (MHT>200) std::cout<<"Warning: event fits in no bin! HT: "<<HT<<", MHT: "<<MHT<<", NJets: "<<NJets<<", BTags: "<<BTags<<std::endl;
      result=999;
    }
  if(match>0)
    {
      std::cout<<"Error event fits in more than one bin! HT: "<<HT<<", MHT: "<<MHT<<", NJets: "<<NJets<<", BTags: "<<BTags<<std::endl;
    }
  return result+1; // to not use the 0 bin but start counting at 1
}

void SearchBins::PrintUsed()
{
  int total = 0;
  for(unsigned int i=0; i< usedBin_.size();i++){
    std::cout<<"Bin["<<i+1<<"] has been used: "<<usedBin_[i]<<std::endl;
    total+=usedBin_[i];;
  }
  std::cout<<"Total: "<<total<<std::endl;
}
