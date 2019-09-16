#include "ClusterEvaluator.h"
#include <fstream>
#include <string>

int main()
{

  // parameters
  unsigned int nombreTotalDeHits=1000;
  unsigned int nombreDeClusters=12;  
  unsigned int nombreDeClusterings=10;
  std::string outputFileName="random_test.txt";
  
  //preparation
  srand(time(NULL));
  unsigned int hitTab[nombreTotalDeHits];
  unsigned int *hitPtr=hitTab;
  unsigned int *clusterPtr=hitPtr;
  std::ofstream outputFile;
  outputFile.open(outputFileName.c_str());

  //running
  HitClusterInfo HCI(nombreDeClusterings,nombreTotalDeHits);
  HCI.setHits(hitPtr,hitPtr+nombreTotalDeHits);
  for (unsigned int ihit=0; ihit<nombreTotalDeHits; ++ihit)
    for (unsigned int iclustering=0; iclustering<nombreDeClusterings; ++iclustering)
      HCI.addClusterCheck(iclustering,hitPtr+ihit,clusterPtr+(rand()%nombreDeClusters));

  std::map<HitClusterInfo::ClusterSetIndices,ClusterPairsDataSums> m=HCI.getAllDataSums();

  for (auto& sum : m) outputFile << sum.second << std::endl;

  //finalising
  outputFile.close();
    
  
  
  return 0;
}
