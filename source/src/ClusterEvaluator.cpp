#include "ClusterEvaluator.h"

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
