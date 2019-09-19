#ifndef HitClusterInfo_LCIO_HH
#define HitClusterInfo_LCIO_HH

#include "HitClusterInfo.h"

#include <string>
#include <vector>

#include "lcio.h"
using namespace lcio ;

#include "IMPL/LCEventImpl.h"

//function to help test the class
LCEventImpl* createEvent(unsigned int With_nCaloHits, const std::vector<std::string>& hitCollectionNames, const std::vector<std::string>& clusterCollectionNames);


class HitClusterInfo_LCIO
{
 public:
  HitClusterInfo_LCIO(const std::vector<std::string>& hitCollectionNames, const std::vector<std::string>& clusterCollectionNames);
  const HitClusterInfo& analyseEvent(const EVENT::LCEvent* evt);
 private:
  std::vector<std::string> m_hitCollectionNames;
  std::vector<std::string> m_clusterCollectionNames;
  //unsigned int m_totalNumberOfHits;
  //unsigned int * m_totalNumerOfClustersPerCollection;
  HitClusterInfo HCI;
};


inline  HitClusterInfo_LCIO::HitClusterInfo_LCIO(const std::vector<std::string>& hitCollectionNames, const std::vector<std::string>& clusterCollectionNames)
  : m_hitCollectionNames(hitCollectionNames), m_clusterCollectionNames(clusterCollectionNames), HCI(clusterCollectionNames.size())
{}


#endif
