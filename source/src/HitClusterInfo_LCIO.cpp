#include "HitClusterInfo_LCIO.h"

#include <stdexcept>

#include "IMPL/LCEventImpl.h"
#include "UTIL/LCTime.h"
#include "IMPL/CalorimeterHitImpl.h"
#include "IMPL/ClusterImpl.h"
#include "IMPL/LCCollectionVec.h"

LCEventImpl* createEvent(unsigned int With_nCaloHits, const std::vector<std::string>& hitCollectionNames, const std::vector<std::string>& clusterCollectionNames)
{
  if (hitCollectionNames.empty()) throw std::logic_error( "createEvent function should be called with at least one name for hit collection" );
  if (clusterCollectionNames.empty()) throw std::logic_error( "createEvent function should be called with at least one name for hit collection" );
  
  static unsigned int eventNumber=0;
  LCEventImpl*  evt = new LCEventImpl();
  
  evt->setRunNumber(10);
  evt->setEventNumber( ++eventNumber );
  LCTime now;
  evt->setTimeStamp( now.timeStamp() );
  evt->setDetectorName( "ClusterTools_test" );

  unsigned int nCaloHitCollections=hitCollectionNames.size();
  LCCollectionVec* calHitCollections[nCaloHitCollections];
  for (unsigned int indexCalCollection=0; indexCalCollection<nCaloHitCollections; ++indexCalCollection)
    calHitCollections[indexCalCollection]=new LCCollectionVec( LCIO::CALORIMETERHIT );

  for (unsigned int ihit=0; ihit<With_nCaloHits; ++ihit)
    {
      CalorimeterHitImpl* calHit = new CalorimeterHitImpl;
      calHitCollections[ihit%nCaloHitCollections]->addElement( calHit );
    }
  
  for (unsigned int indexCalCollection=0; indexCalCollection<nCaloHitCollections; ++indexCalCollection)
    evt->addCollection( calHitCollections[indexCalCollection], hitCollectionNames[indexCalCollection] );
  
  return evt;
}
