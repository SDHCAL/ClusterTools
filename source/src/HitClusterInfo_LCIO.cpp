#include "HitClusterInfo_LCIO.h"

#include <stdexcept>

#include "IMPL/LCEventImpl.h"
#include "UTIL/LCTime.h"
#include "UTIL/BitSet32.h"
#include "IMPL/CalorimeterHitImpl.h"
#include "IMPL/ClusterImpl.h"
#include "IMPL/LCCollectionVec.h"

LCEventImpl* createEvent(unsigned int With_nCaloHits, const std::vector<std::string>& hitCollectionNames, const std::vector<std::string>& clusterCollectionNames)
{
  if (hitCollectionNames.empty()) throw std::logic_error( "createEvent function should be called with at least one name for hit collection" );
  if (clusterCollectionNames.empty()) throw std::logic_error( "createEvent function should be called with at least one name for cluster collection" );
  
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

  
  unsigned int nClusterCollections=clusterCollectionNames.size();
  LCCollectionVec* clusterCollections[nClusterCollections];
  for (unsigned int indexClusterCollection=0; indexClusterCollection<nClusterCollections; ++indexClusterCollection)
    {
      clusterCollections[indexClusterCollection]=new LCCollectionVec( LCIO::CLUSTER ); //create cluster Collection
      clusterCollections[indexClusterCollection]->setFlag( UTIL::make_bitset32( LCIO::CLBIT_HITS )); //turn on clusters to point back to the hits
      unsigned int nClustersInCollection=indexClusterCollection+1;
      ClusterImpl* clusters[nClustersInCollection];
      for (unsigned int indexCluster=0; indexCluster<nClustersInCollection; ++indexCluster) //create clusters
	  clusters[indexCluster]=new ClusterImpl;
      
      for (unsigned int indexCalCollection=0; indexCalCollection<nCaloHitCollections; ++indexCalCollection)
	for (unsigned indexHit=0; indexHit<calHitCollections[indexCalCollection]->size(); ++indexHit)
	  {
	    clusters[(indexCalCollection+indexHit)%nClustersInCollection]->addHit( dynamic_cast<CalorimeterHit*>( calHitCollections[indexCalCollection]->getElementAt(indexHit) ), 1.0*indexCalCollection) ;
	  }

      for (unsigned int indexCluster=0; indexCluster<nClustersInCollection; ++indexCluster) //add clusters to collection
	clusterCollections[indexClusterCollection]->addElement( clusters[indexCluster] );
    }


  for (unsigned int indexClusterCollection=0; indexClusterCollection<nClusterCollections; ++indexClusterCollection)
    evt->addCollection( clusterCollections[indexClusterCollection], clusterCollectionNames[indexClusterCollection] );

  
  return evt;
}



const HitClusterInfo& HitClusterInfo_LCIO::analyseEvent(const EVENT::LCEvent* evt)
{
  std::vector<LCCollection*> TheClusterCollections;
  for (auto& collectionName : m_clusterCollectionNames)
    {
      LCCollection* clusterCol=evt->getCollection( collectionName.c_str() );
      int flag=clusterCol->getFlag();
      if ( UTIL::BitSet32( flag )[LCIO::CLBIT_HITS] )
	TheClusterCollections.push_back( clusterCol );
      else
	throw std::logic_error("HitClusterInfo_LCIO::analyseEvent : cluster collection should have LCIO::CLBIT_HITS set to get hit information");
    }
  
  std::vector<LCCollectionVec*> TheHitCollections;
  unsigned int nHitTotal=0;
  for (auto& collectionName :  m_hitCollectionNames)
    {
      try
	{
	  LCCollection *col=evt->getCollection( collectionName.c_str() );
	  TheHitCollections.push_back( dynamic_cast<LCCollectionVec*>(col) );
	  nHitTotal+=col->getNumberOfElements();
	}
      catch (DataNotAvailableException&)
	{
	  if (m_throwErrorIfHitCollectionIsNotFound) throw;
	}
    }
  HCI.reset(nHitTotal);
  for (auto& collectionPointer : TheHitCollections) HCI.setHitsFromPointers(*collectionPointer,false);
  
  for (unsigned int indexClustering=0; indexClustering<TheClusterCollections.size(); ++ indexClustering)
    {
      LCCollection* currentClusterCollection=TheClusterCollections[indexClustering];
      for (unsigned int indexCluster=0; indexCluster<currentClusterCollection->getNumberOfElements(); ++indexCluster)
	{
	  LCObject* currentObj= currentClusterCollection->getElementAt(indexCluster);
	  Cluster* currentCluster=dynamic_cast<Cluster*>( currentObj );
	  const CalorimeterHitVec& thisClusterHits=currentCluster->getCalorimeterHits();
	  for (auto& HitPointer : thisClusterHits)
	    HCI.addCluster(indexClustering,HitPointer,currentCluster,m_canAddHitFromClusters); 
	}
    }
  
  return HCI;
}
