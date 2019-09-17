#ifndef ClusterEvaluator_HH
#define ClusterEvaluator_HH

#include <vector>
#include <map>
#include <utility>
#include <stdexcept>

#include "ClusterPairsDataSums.h"

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
  ClusterPairsDataSums getDataSums(unsigned int firstClusterSetIndex,unsigned int secondClusterSetIndex) const;
  ClusterPairsDataSums getDataSumsCheck(unsigned int firstClusterSetIndex,unsigned int secondClusterSetIndex) const;
  
  typedef std::pair<unsigned int,unsigned int> ClusterSetIndices;
  std::map<ClusterSetIndices,ClusterPairsDataSums> getAllDataSums() const;
  
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

inline ClusterPairsDataSums HitClusterInfo::getDataSumsCheck(unsigned int firstClusterSetIndex,unsigned int secondClusterSetIndex) const
{
  checkClusterIndex(firstClusterSetIndex);
  checkClusterIndex(secondClusterSetIndex);
  return getDataSums(firstClusterSetIndex,secondClusterSetIndex);
}
#endif
