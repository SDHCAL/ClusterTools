#ifndef ClusterEvaluator_HH
#define ClusterEvaluator_HH

#include <vector>
#include <map>
#include <utility>
#include <math.h>
#include <stdexcept>

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
  unsigned int m_numberOfCoGroupedHitPair_InBothClusterSets=0;
  unsigned int m_numberOfCoGroupedHitPair_OnlyInFirstClusterSet=0;
  unsigned int m_numberOfCoGroupedHitPair_OnlyInSedondClusterSet=0;
  unsigned int m_numberOfNonCoGroupedHitPair_InBothClusterSets=0;
  friend class HitClusterInfo;
};


class HitClusterInfo
{
 public:
  HitClusterInfo(unsigned int nClusters=2,unsigned int nhits=0);

  // methods to add hits
  void addHit(const void * hit); 
  bool addHitCheck(const void * hit);
  template <class Titer>
    void setHits(Titer begin, Titer end);
  template <class Container>
    void setHits(const Container& c) {setHits(c.begin(),c.end());}
  
  bool checkHitUnicity() const;

  //methods to add clusters
  bool addCluster(unsigned int partitionNumber,const void *hit, const void *clusterPointer, bool addTheHit=false);
  bool addClusterCheck(unsigned int partitionNumber,const void *hit, const void *clusterPointer, bool addTheHit=false);

  //methods to create and fill ClusterPairsDataSums
  ClusterPairsDataSums getDataSums(unsigned int firstClusterSetIndex,unsigned int secondClusterSetIndex);
  ClusterPairsDataSums getDataSumsCheck(unsigned int firstClusterSetIndex,unsigned int secondClusterSetIndex);
  
  typedef std::pair<unsigned int,unsigned int> ClusterSetIndices;
  std::map<ClusterSetIndices,ClusterPairsDataSums> getAllDataSums();
  
  unsigned int containerSize() const;
 private:
  unsigned int m_numberOfClusteringCases;
  unsigned int m_skip;
  std::vector<const void *> m_pointersToHits_and_Clusters;

  std::vector<const void *>::const_iterator find(const void * hit) const;
  std::vector<const void *>::iterator find(const void * hit);

  void checkClusterIndex(unsigned int clusterIndex) const;

};

inline unsigned int HitClusterInfo::containerSize() const
{
  return m_pointersToHits_and_Clusters.size();
}

inline void HitClusterInfo::addHit(const void * hit)
{
  m_pointersToHits_and_Clusters.push_back(hit);
  m_pointersToHits_and_Clusters.resize(m_pointersToHits_and_Clusters.size()+m_numberOfClusteringCases,nullptr);
}

inline bool HitClusterInfo::addHitCheck(const void * hit)
{
  bool added=(find(hit)==m_pointersToHits_and_Clusters.end());
  if (added) addHit(hit);
  return added;
}

template<class Titer>
void HitClusterInfo::setHits(Titer begin, Titer end)
{
  const void ** itvec= m_pointersToHits_and_Clusters.data();
  for (Titer it=begin; it!=end; ++it) { (*itvec)=&(*it); itvec+= m_skip;}
}

inline void HitClusterInfo::checkClusterIndex(unsigned int clusterIndex) const
{
  if (clusterIndex>=m_numberOfClusteringCases)
    throw std::range_error("HitClusterInfo called with too high cluster index number");
}

inline bool HitClusterInfo::addClusterCheck(unsigned int partitionNumber,const void *hit, const void *clusterPointer, bool addTheHit)
{
  checkClusterIndex(partitionNumber);
  addCluster(partitionNumber,hit,clusterPointer,addTheHit);
}

inline ClusterPairsDataSums HitClusterInfo::getDataSumsCheck(unsigned int firstClusterSetIndex,unsigned int secondClusterSetIndex)
{
  checkClusterIndex(firstClusterSetIndex);
  checkClusterIndex(secondClusterSetIndex);
  return getDataSums(firstClusterSetIndex,secondClusterSetIndex);
}
#endif
