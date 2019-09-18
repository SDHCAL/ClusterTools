#include "HitClusterInfo.h"

#include <stdexcept>
#include <set>



HitClusterInfo::HitClusterInfo(unsigned int nClusters,unsigned int nhits) :
  m_numberOfClusteringCases(nClusters),m_skip(nClusters+1),
  m_pointersToHits_and_Clusters((1+nClusters)*nhits,nullptr)
{
  if (nClusters<2) throw std::domain_error(" HitClusterInfo construction : minimum number of clusters is 2");
}


std::vector<const void *>::const_iterator HitClusterInfo::find(const void * hit) const
{
  for (std::vector<const void *>::const_iterator it=m_pointersToHits_and_Clusters.begin();
       it < m_pointersToHits_and_Clusters.end();
       it+=m_skip)
    if ( (*it)==hit) return it;
  return m_pointersToHits_and_Clusters.end();
}

// I know, it's duplicated code but I haven't figured out how to avoid it in proper C++ coding
std::vector<const void *>::iterator HitClusterInfo::find(const void * hit) 
{
  for (std::vector<const void *>::iterator it=m_pointersToHits_and_Clusters.begin();
       it < m_pointersToHits_and_Clusters.end();
       it+=m_skip)
    if ( (*it)==hit) return it;
  return m_pointersToHits_and_Clusters.end();
}

bool HitClusterInfo::checkHitUnicity() const
{
  std::set<const void *> hitSet;
  for (std::vector<const void *>::const_iterator it=m_pointersToHits_and_Clusters.begin();
       it < m_pointersToHits_and_Clusters.end();
       it+=m_skip)
    hitSet.insert(*it);
  return hitSet.size()==containerSize()/m_skip;
}


bool HitClusterInfo::addCluster(unsigned int partitionNumber,const void *hit, const void *clusterPointer,  bool addTheHit)
{
  std::vector<const void *>::iterator it=find(hit);
  bool hitfound=(it!=m_pointersToHits_and_Clusters.end());
  if (! hitfound)
    {
      if (addTheHit) {addHit(hit); it=find(hit);}
      else throw std::domain_error("HitClusterInfo::addCluster, hit not found");
    }
  *(it+(1+partitionNumber))=clusterPointer;
  return hitfound;
}


ClusterPairsDataSums HitClusterInfo::getDataSums(unsigned int firstClusterSetIndex,unsigned int secondClusterSetIndex) const
{
  //std::cout << "printout of " << m_pointersToHits_and_Clusters.size() << " hits and clusters : ";
  //for (auto p : m_pointersToHits_and_Clusters) std::cout << p << " ";
  //std::cout<<std::endl;
  //
  ClusterPairsDataSums result;
  unsigned int firstCluster=1+firstClusterSetIndex;
  unsigned int secondCluster=1+secondClusterSetIndex;
  const void* const* pbegin=m_pointersToHits_and_Clusters.data();
  const void* const* pend=pbegin+m_pointersToHits_and_Clusters.size();
  for (const void* const* p1=pbegin; p1<pend; p1+=m_skip)
    for (const void* const* p2=p1+m_skip; p2<pend; p2+=m_skip)
      if (*(p1+firstCluster)==*(p2+firstCluster))
	{
	  if (*(p1+secondCluster)==*(p2+secondCluster)) ++result.m_numberOfCoGroupedHitPair_InBothClusterSets;
	  else ++result.m_numberOfCoGroupedHitPair_OnlyInFirstClusterSet;
	}
      else
	{
	  if (*(p1+secondCluster)==*(p2+secondCluster)) ++result.m_numberOfCoGroupedHitPair_OnlyInSedondClusterSet;
	  else ++result.m_numberOfNonCoGroupedHitPair_InBothClusterSets;
	}
  return result;
}

std::map<HitClusterInfo::ClusterSetIndices,ClusterPairsDataSums> HitClusterInfo::getAllDataSums() const
{
  std::map<ClusterSetIndices,ClusterPairsDataSums> m;
  for (unsigned int i=0; i<m_numberOfClusteringCases; ++i)
    for (unsigned int j=i+1; j<m_numberOfClusteringCases; ++j)
      m[std::make_pair(i,j)]=getDataSums(i,j);
  return m;
}
