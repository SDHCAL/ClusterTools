#include "HitClusterInfo_LCIO.h"

#include <assert.h>
#include <iostream>

#include "UTIL/LCTOOLS.h"


int main()
{
  std::vector<std::string> CaloHitCollectionNames;
  std::vector<std::string> ClusterCollectionNames;
  LCEventImpl* evt=nullptr;
  
  bool exceptionThrown = false;
  try
    {
      evt=createEvent(3,CaloHitCollectionNames,ClusterCollectionNames);
    }
  catch (std::logic_error&)
    {
      exceptionThrown = true;
    }
  assert(exceptionThrown);

  CaloHitCollectionNames.push_back("AirHit_One");
  CaloHitCollectionNames.push_back("AirHit_Two");

  exceptionThrown = false;
  try
    {
      evt=createEvent(3,CaloHitCollectionNames,ClusterCollectionNames);
    }
  catch (std::logic_error&)
    {
      exceptionThrown = true;
    }
  assert(exceptionThrown);

  ClusterCollectionNames.push_back("Cluster_One");
  ClusterCollectionNames.push_back("Cluster_Two");
  evt=createEvent(3,CaloHitCollectionNames,ClusterCollectionNames);

  assert(evt->getCollectionNames()->size()==CaloHitCollectionNames.size()+ClusterCollectionNames.size());
  LCTOOLS::dumpEventDetailed(evt);
  delete evt;
  return 0;
}
