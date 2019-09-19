#include "HitClusterInfo_LCIO.h"

#include <assert.h>
#include <iostream>

#include "UTIL/LCTOOLS.h"


int main()
{
  const std::vector<std::string> EmptyCol;
  const std::vector<std::string> DummyColName(2,"dummy");
  std::vector<std::string> CaloHitCollectionNames;
  std::vector<std::string> ClusterCollectionNames;
  LCEventImpl* evt=nullptr;
  
  bool exceptionThrown = false;
  try { evt=createEvent(3,CaloHitCollectionNames,ClusterCollectionNames); }
  catch (std::logic_error&) {exceptionThrown = true;}
  assert(exceptionThrown);

  CaloHitCollectionNames.push_back("AirHit_One");
  CaloHitCollectionNames.push_back("AirHit_Two");

  exceptionThrown = false;
  try{ evt=createEvent(3,CaloHitCollectionNames,ClusterCollectionNames);}
  catch (std::logic_error&){exceptionThrown = true;}
  assert(exceptionThrown);

  ClusterCollectionNames.push_back("Cluster_One");
  ClusterCollectionNames.push_back("Cluster_Two");
  evt=createEvent(3,CaloHitCollectionNames,ClusterCollectionNames);

  assert(evt->getCollectionNames()->size()==CaloHitCollectionNames.size()+ClusterCollectionNames.size());
  LCTOOLS::dumpEventDetailed(evt);

  exceptionThrown = false;
  try { HitClusterInfo_LCIO dummyHCI(CaloHitCollectionNames,EmptyCol);}
  catch (std::domain_error&) {exceptionThrown = true;}
  assert(exceptionThrown);

  HitClusterInfo_LCIO HCI_lcio_test(DummyColName,ClusterCollectionNames);
  HCI_lcio_test.allowsAddingHitsFromCluster();
  exceptionThrown = false;
  try {HCI_lcio_test.analyseEvent(evt);} catch (DataNotAvailableException&) {exceptionThrown = true;}
  assert(! exceptionThrown);
  HCI_lcio_test.forbidsMissingHitCollection();
  try {HCI_lcio_test.analyseEvent(evt);} catch (DataNotAvailableException&) {exceptionThrown = true;}
  assert(exceptionThrown);
  exceptionThrown = false;
  HCI_lcio_test.allowsMissingHitCollection();
  try {HCI_lcio_test.analyseEvent(evt);} catch (DataNotAvailableException&) {exceptionThrown = true;}
  assert(! exceptionThrown);


  HitClusterInfo_LCIO HCI_lcio_testbis(CaloHitCollectionNames,DummyColName);
  exceptionThrown = false;
  try {HCI_lcio_testbis.analyseEvent(evt);} catch (DataNotAvailableException&) {exceptionThrown = true;}
  assert(exceptionThrown);

  HitClusterInfo_LCIO HCI_lcio_noHits(EmptyCol,ClusterCollectionNames);
  HitClusterInfo_LCIO HCI_lcio(CaloHitCollectionNames,ClusterCollectionNames);
    
  const HitClusterInfo& HCI_noHits=HCI_lcio_noHits.analyseEvent(evt);

  const HitClusterInfo& HCI=HCI_lcio.analyseEvent(evt); //crash
  assert(HCI.numberOfHits()==3);
  
  delete evt;
  return 0;
}
