#include "HitClusterInfo_LCIO.h"

#include "IO/LCWriter.h"

int main(int argc, char **argv)
{
  std::vector<std::string> CaloHitCollectionNames;
  std::vector<std::string> ClusterCollectionNames;
  
  LCWriter* lcWrt = LCFactory::getInstance()->createLCWriter();
  lcWrt->open("ClusterTools_test.slcio", LCIO::WRITE_NEW );

  LCEventImpl* evt=createEvent(3,CaloHitCollectionNames,ClusterCollectionNames);
  lcWrt->writeEvent( evt );
  delete evt;

  lcWrt->close();
  delete lcWrt;

  return 0;
};
