#ifndef ClusterEvaluator_HH
#define ClusterEvaluator_HH

#include <vector>
#include <math.h>

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
  double RandIndex() const {return (TP()+TN())/1.0d/(TP()+FP()+FN()+TN());}
  double JacardIndex() const {return TP()/1.0d/(TP()+FP()+FN());}
  double DiceIndex() const {return 2*TP()/1.0d/(2*TP()+FP()+FN());}
  double Precision() const {return TP()/1.0d/(TP()+FP());}
  double Recall() const {return TP()/1.0d/(TP()+FN());}
  double Fmeasure(double beta) const {return ((beta*beta+1)*Precision()*Recall())/(beta*beta*Precision()+Recall());}
  double FowlkesMallowsIndex() const {return sqrt(Precision()*Recall());}
  
 private:
  unsigned int m_numberOfCoGroupedHitPair_InBothClusterSets;
  unsigned int m_numberOfCoGroupedHitPair_OnlyInFirstClusterSet;
  unsigned int m_numberOfCoGroupedHitPair_OnlyInSedondClusterSet;
  unsigned int m_numberOfNonCoGroupedHitPair_InBothClusterSets;
};


class HitClusterInfo
{
 public:
  HitClusterInfo(unsigned int nClusters=2,unsigned int nhits=0);
 private:
  unsigned int m_numberOfClusteringCases;
  std::vector<const void *> m_pointersToHits_and_Clusters;
};

#endif
