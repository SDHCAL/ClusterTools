#include "HitClusterInfo_LCIO.h"

#include "IMPL/LCEventImpl.h"
#include "UTIL/LCTime.h"
#include "IMPL/CalorimeterHitImpl.h"
#include "IMPL/ClusterImpl.h"
#include "IMPL/LCCollectionVec.h"

LCEventImpl* createEvent(unsigned int With_nCaloHits, const std::vector<std::string>& hitCollectionNames, const std::vector<std::string>& clusterCollectionNames)
{
  static unsigned int eventNumber=0;
  LCEventImpl*  evt = new LCEventImpl();
  
  evt->setRunNumber(10);
  evt->setEventNumber( ++eventNumber );
  LCTime now;
  evt->setTimeStamp( now.timeStamp() );
  evt->setDetectorName( "ClusterTools_test" );
  
  return evt;
}
