#ifndef HitClusterInfo_HH
#define HitClusterInfo_HH

#include <vector>
#include <map>
#include <utility>
#include <stdexcept>

#include "ClusterPairsDataSums.h"

class HitClusterInfo
{
 public:
  HitClusterInfo(unsigned int nClusters=2,unsigned int nhits=0);

  //reset with allocate storage for nhits
  void reset(unsigned int nhits);
  
  // methods to add hits
  void addHit(const void * hit); 
  bool addHitCheck(const void * hit);
  //method to set hits by block.
  // WARNING : the setHits and  setHitsFromPointers methods don't allocate space in the container
  // It is the caller responsability to make sure enough hits have been allocated in HitClusterInfo
  //
  // The SetHits methods are for cases where the container contains the hit objects, the iterator are pointers to hit objects
  // When called, the pointers to the objects (stored in the container) are retrieved by &(*iterator)
  //
  // The setHitsFromPointers methods are for cases where the container contains pointers to the hit objects, the iterator are double pointers to hit objects
  // When called, the pointers (stored in the container) to the objects  are retrieved by *iterator
  // This is notably the case for LCIO's LCCollectionVec which is a std::vector<LCObject*>
  template <class Titer>
    void setHits(Titer begin, Titer end, bool rewind=true);
  template <class Container>
    void setHits(const Container& c, bool rewind=true) {setHits(c.begin(),c.end(),rewind);}
  template <class Titer>
    void setHitsFromPointers(Titer begin, Titer end, bool rewind=true); 
  template <class Container>
    void setHitsFromPointers(const Container& c, bool rewind=true) {setHitsFromPointers(c.begin(),c.end(),rewind);}
  bool checkHitUnicity() const;

  //methods to add clusters
  bool addCluster(unsigned int partitionNumber,const void *hit, const void *clusterPointer, bool addTheHit=false);
  bool addClusterCheck(unsigned int partitionNumber,const void *hit, const void *clusterPointer, bool addTheHit=false);

  //methods to create and fill ClusterPairsDataSums
  ClusterPairsDataSums getDataSums(unsigned int firstClusterSetIndex,unsigned int secondClusterSetIndex) const;
  ClusterPairsDataSums getDataSumsCheck(unsigned int firstClusterSetIndex,unsigned int secondClusterSetIndex) const;
  
  typedef std::pair<unsigned int,unsigned int> ClusterSetIndices;
  std::map<ClusterSetIndices,ClusterPairsDataSums> getAllDataSums() const;
  
  unsigned int numberOfHits() const;
  std::vector<unsigned int> numberOfClustersPerClustering() const;
 private:
  unsigned int m_numberOfClusteringCases;
  unsigned int m_skip;
  std::vector<const void *> m_pointersToHits_and_Clusters;
  const void ** iterator_memory_for_set_hit;

  void rewind_iterator_memory_for_set_hit(); 
  std::vector<const void *>::const_iterator find(const void * hit) const;
  std::vector<const void *>::iterator find(const void * hit);

  void checkClusterIndex(unsigned int clusterIndex) const;

};


inline unsigned int HitClusterInfo::numberOfHits() const
{
  return m_pointersToHits_and_Clusters.size()/m_skip;
}

inline void HitClusterInfo::rewind_iterator_memory_for_set_hit()
{
  iterator_memory_for_set_hit=m_pointersToHits_and_Clusters.data();
}


inline void HitClusterInfo::addHit(const void * hit)
{
  m_pointersToHits_and_Clusters.push_back(hit);
  m_pointersToHits_and_Clusters.resize(m_pointersToHits_and_Clusters.size()+m_numberOfClusteringCases,nullptr);
  rewind_iterator_memory_for_set_hit();
}

inline bool HitClusterInfo::addHitCheck(const void * hit)
{
  bool added=(find(hit)==m_pointersToHits_and_Clusters.end());
  if (added) addHit(hit);
  return added;
}

template<class Titer>
void HitClusterInfo::setHits(Titer begin, Titer end, bool rewind)
{
  if (rewind) rewind_iterator_memory_for_set_hit();
  for (Titer it=begin; it!=end; ++it)  {(*iterator_memory_for_set_hit)=&(*it); iterator_memory_for_set_hit+= m_skip;}
}

template<class Titer>
void HitClusterInfo::setHitsFromPointers(Titer begin, Titer end, bool rewind)
{
  if (rewind) rewind_iterator_memory_for_set_hit();
  for (Titer it=begin; it!=end; ++it)  {(*iterator_memory_for_set_hit)=*it; iterator_memory_for_set_hit+= m_skip;}
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


inline void HitClusterInfo::reset(unsigned int nhits)
{
  m_pointersToHits_and_Clusters.assign(nhits*m_skip,nullptr);
  rewind_iterator_memory_for_set_hit();
}
#endif
