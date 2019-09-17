#ifndef ClusterPairsDataSums_HH
#define ClusterPairsDataSums_HH

#include <math.h>
#include <iostream>

class ClusterPairsDataSums
{
 public:
  //long explicit name getter
  unsigned int numberOfCoGroupedHitPair_InBothClusterSets() const {return m_numberOfCoGroupedHitPair_InBothClusterSets;}  //a, TP
  unsigned int numberOfCoGroupedHitPair_OnlyInFirstClusterSet() const {return m_numberOfCoGroupedHitPair_OnlyInFirstClusterSet;} //c, FN
  unsigned int numberOfCoGroupedHitPair_OnlyInSedondClusterSet() const {return m_numberOfCoGroupedHitPair_OnlyInSedondClusterSet;} // d, FP
  unsigned int numberOfNonCoGroupedHitPair_InBothClusterSets() const {return m_numberOfNonCoGroupedHitPair_InBothClusterSets;} // b, TN

  //wikipedia Rand Index labelling
  unsigned int a() const {return numberOfCoGroupedHitPair_InBothClusterSets();}
  unsigned int b() const {return numberOfNonCoGroupedHitPair_InBothClusterSets();}
  unsigned int c() const {return numberOfCoGroupedHitPair_OnlyInFirstClusterSet();}
  unsigned int d() const {return numberOfCoGroupedHitPair_OnlyInSedondClusterSet();}
  
  //wikipedia Cluster_analysis#External_evaluation labelling
  //Here first cluster set in assumed to be a truth reference
  //Second cluster is the tested against
  unsigned int NumberOfTruePositive() const {return numberOfCoGroupedHitPair_InBothClusterSets();}
  unsigned int TP() const {return NumberOfTruePositive();}
  unsigned int NumberOfTrueNegative() const {return numberOfNonCoGroupedHitPair_InBothClusterSets();}
  unsigned int TN() const {return NumberOfTrueNegative();}
  unsigned int NumberOfFalsePostive() const {return numberOfCoGroupedHitPair_OnlyInSedondClusterSet();}
  unsigned int FP() const {return NumberOfFalsePostive();}
  unsigned int NumberOfFalseNegative() const {return numberOfCoGroupedHitPair_OnlyInFirstClusterSet();}
  unsigned int FN() const {return NumberOfFalseNegative();}

  //the measures
  double RandIndex() const {return (TP()+TN())/1.0/(TP()+FP()+FN()+TN());}
  double JacardIndex() const {return TP()/1.0/(TP()+FP()+FN());}
  double DiceIndex() const {return 2*TP()/1.0/(2*TP()+FP()+FN());}
  double Precision() const {return TP()/1.0/(TP()+FP());}
  double Recall() const {return TP()/1.0/(TP()+FN());}
  double Fmeasure(double beta) const {return ((beta*beta+1)*Precision()*Recall())/(beta*beta*Precision()+Recall());}
  double FowlkesMallowsIndex() const {return sqrt(Precision()*Recall());}


  //setter
  void set(unsigned int both, unsigned int onlyFirst, unsigned int onlySecond, unsigned int none);
 private:
  unsigned int m_numberOfCoGroupedHitPair_InBothClusterSets=0;
  unsigned int m_numberOfCoGroupedHitPair_OnlyInFirstClusterSet=0;
  unsigned int m_numberOfCoGroupedHitPair_OnlyInSedondClusterSet=0;
  unsigned int m_numberOfNonCoGroupedHitPair_InBothClusterSets=0;
  friend class HitClusterInfo;
};

std::ostream& operator<<(std::ostream& flux, const ClusterPairsDataSums);
std::istream& operator>>(std::istream& flux, ClusterPairsDataSums&);

#ifdef BUILD_WITH_ROOT
class TTree;
TTree* ClusterPairsDataSums_ASCIItoTTree(const char *inputfilename, const char* treeName="DataSums");
#endif

#endif
