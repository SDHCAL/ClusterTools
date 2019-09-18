#include "HitClusterInfo_LCIO.h"

#include <assert.h>
#include <iostream>



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

  CaloHitCollectionNames.push_back("Hilbert");
  CaloHitCollectionNames.push_back("Space");

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

  ClusterCollectionNames.push_back("Clark");
  ClusterCollectionNames.push_back("Kent");
  evt=createEvent(3,CaloHitCollectionNames,ClusterCollectionNames);

  //for the moment, only two collections, waiting to implement clusters
  std::cout << "number of collections=" << evt->getCollectionNames()->size() << std::endl;
  delete evt;
  return 0;
}
