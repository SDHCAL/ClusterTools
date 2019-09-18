#include "HitClusterInfo_LCIO.h"

#include <assert.h>
#include <iostream>



int main()
{
  std::vector<std::string> CaloHitCollectionNames;
  std::vector<std::string> ClusterCollectionNames;
  LCEventImpl* evt=createEvent(3,CaloHitCollectionNames,ClusterCollectionNames);
  
  std::cout << "number of collections=" << evt->getCollectionNames()->size() << std::endl;
  delete evt;
  return 0;
}
