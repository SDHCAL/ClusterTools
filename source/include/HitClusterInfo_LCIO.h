#ifndef HitClusterInfo_LCIO_HH
#define HitClusterInfo_LCIO_HH

#include <string>
#include <vector>

#include "lcio.h"
using namespace lcio ;

#include "IMPL/LCEventImpl.h"

//function to help test the class
LCEventImpl* createEvent(unsigned int With_nCaloHits, const std::vector<std::string>& hitCollectionNames, const std::vector<std::string>& clusterCollectionNames);

/*
class HitClusterInfo_LCIO
{
};
*/

#endif
