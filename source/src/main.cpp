#include "HitClusterInfo.h"
#include <fstream>
#include <iostream>
#include <string>
#include <getopt.h>

void usage(char* progname)
{
  std::cout << "Usage :" << progname << " takes the following options : " << std::endl;
  std::cout << "\t -n X or --number_of_hits=X : specify the number of hits to cluster (default is 1000)"  << std::endl;
  std::cout << "\t -c X or --number_of_clusters : specify the maximum number of clusters to make (default is 12)" << std::endl;
  std::cout << "\t -C X or --number_of_clusterings : specify the number of random clustering to perform (default is 10)" << std::endl;
  std::cout << "\t -r X or --number_of_repeats : specify the number of times to repet all the random clusterings" << std::endl;
  std::cout << "\t -f string or --output_file_name=string : the output file name (default is random_test.txt)"  << std::endl;
  std::cout << "\t -h or --help : print this help and exit"  << std::endl;
}

int main(int argc, char **argv)
{

  // parameters
  unsigned int nombreTotalDeHits=1000;
  unsigned int nombreDeClusters=12;  
  unsigned int nombreDeClusterings=10;
  unsigned int nombreDeRepetitions=1;
  std::string outputFileName="random_test.txt";

  //option decoding
  static struct option long_options[] = {
    {"number_of_hits",        required_argument, 0,  'n' },
    {"number_of_clusters",    required_argument, 0,  'c' },
    {"number_of_clusterings", required_argument, 0,  'C' },
    {"number_of_repeats",     required_argument, 0,  'r' },
    {"output_file_name",      required_argument, 0,  'f' },
    {"help",                  no_argument,       0,  'h' },
    {0,                       0,                 0,  0   }
  };

  int long_index =0;
  int opt= 0;
  while ((opt = getopt_long(argc, argv,"n:c:C:r:f:h",long_options, &long_index )) != -1)
    {
      switch(opt)
	{
	case 'n' : nombreTotalDeHits=atoi(optarg); break;
	case 'c' : nombreDeClusters=atoi(optarg); break;
	case 'C' : nombreDeClusterings=atoi(optarg); break;
	case 'r' : nombreDeRepetitions=atoi(optarg); break;
	case 'f' : outputFileName=optarg; break;
	case 'h' : usage(argv[0]); return 0;
	default  : usage(argv[0]); return 1;
	}
    }

  
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
  for (unsigned int irepeat=0; irepeat<nombreDeRepetitions; ++irepeat)
    {
      for (unsigned int ihit=0; ihit<nombreTotalDeHits; ++ihit)
	for (unsigned int iclustering=0; iclustering<nombreDeClusterings; ++iclustering)
	  HCI.addClusterCheck(iclustering,hitPtr+ihit,clusterPtr+(rand()%nombreDeClusters));

      std::map<HitClusterInfo::ClusterSetIndices,ClusterPairsDataSums> m=HCI.getAllDataSums();

      for (auto& sum : m) outputFile << sum.second << std::endl;
    }
  //finalising
  outputFile.close();
    
  
  
  return 0;
}
