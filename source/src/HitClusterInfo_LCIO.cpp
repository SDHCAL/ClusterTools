#include "HitClusterInfo_LCIO.h"

#include "IMPL/LCEventImpl.h"

LCEventImpl* createEvent(unsigned int With_nCaloHits, const std::vector<std::string>& hitCollectionNames, const std::vector<std::string>& clusterCollectionNames)
{
  LCEventImpl*  evt = new LCEventImpl();
  return evt;
}
