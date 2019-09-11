#ifndef ClusterEvaluator_HH
#define ClusterEvaluator_HH

#include <vector>

class HitClusterInfo
{
 public:
  HitClusterInfo(unsigned int nClusters=2,unsigned int nhits=0);
 private:
  unsigned int m_numberOfClusteringCases;
  std::vector<const void *> m_pointersToHits_and_Clusters;
};

#endif
