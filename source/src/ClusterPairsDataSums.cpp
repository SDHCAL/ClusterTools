#include "ClusterPairsDataSums.h"

void ClusterPairsDataSums::set(unsigned int both, unsigned int onlyFirst, unsigned int onlySecond, unsigned int none)
{
  m_numberOfCoGroupedHitPair_InBothClusterSets=both;
  m_numberOfCoGroupedHitPair_OnlyInFirstClusterSet=onlyFirst;
  m_numberOfCoGroupedHitPair_OnlyInSedondClusterSet=onlySecond;
  m_numberOfNonCoGroupedHitPair_InBothClusterSets=none;
}

std::ostream& operator<<(std::ostream& flux, const ClusterPairsDataSums obj)
{
  flux << obj.numberOfCoGroupedHitPair_InBothClusterSets() << " "
       << obj.numberOfCoGroupedHitPair_OnlyInFirstClusterSet() << " "
       << obj.numberOfCoGroupedHitPair_OnlyInSedondClusterSet() << " "
       << obj.numberOfNonCoGroupedHitPair_InBothClusterSets();
  return flux;
}

std::istream& operator>>(std::istream& flux, ClusterPairsDataSums& obj)
{
  unsigned int both,onlyFirst,onlySecond,none;
  flux >> both >> onlyFirst >> onlySecond >> none;
  obj.set(both,onlyFirst,onlySecond,none);
  return flux;
}
