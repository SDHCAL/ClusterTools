#include "ClusterEvaluator.h"

#include <stdexcept>

HitClusterInfo::HitClusterInfo(unsigned int nClusters,unsigned int nhits) :
  m_numberOfClusteringCases(nClusters),
  m_pointersToHits_and_Clusters((1+nClusters)*nhits,nullptr)
{
  if (nClusters<2) throw std::domain_error(" HitClusterInfo construction : minimum number of clusters is 2");
}

