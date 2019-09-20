#include "HitClusterInfo_LCIO.h"

#include <iostream>
#include <getopt.h>

//#include "IO/LCWriter.h"

void usage(char* progname)
{
   std::cout << "Usage :" << progname << " takes the following options : " << std::endl;
   std::cout << "\t -c X or --calohit_col_name : white space separated list of LCIO CalorimeterHit collections names. (default is none)" << std::endl;
   std::cout << "\t -C X or --cluster_col_name  : white space separated list of LCIO Cluster collections names. (default is none)" << std::endl;
   std::cout << "\t \t \t \t  You need to provide at last 2 Cluster collection names to runs clustering comparisons" << std::endl;
   std::cout << "\t -f string or --output_file_name=string : the output file name. (default is ....)"  << std::endl;
   std::cout << "\t -h or --help : print this help and exit"  << std::endl;
   std::cout << "Examples:" <<std::endl;
   std::cout << "\t" << progname << " -c hcolOne hcolTwo hcolThree -C ClusColOne ClusColTwo" << std::endl;
   std::cout << "\t" << progname << " -c hcolOne -C ClusColOne ClusColTwo -c hcolBisOne hcolBisTwo" << std::endl;
}


int main(int argc, char **argv)
{
  // parameters
  std::vector<std::string> CaloHitCollectionNames;
  std::vector<std::string> ClusterCollectionNames;
  std::string outputFileName="ClusterTools_test_lcio_write.slcio";
  
  //option decoding
  static struct option long_options[] = {
    {"calohit_col_name",      required_argument, 0,  'c' },
    {"cluster_col_name",      required_argument, 0,  'C' },
    {"output_file_name",      required_argument, 0,  'f' },
    {"help",                  no_argument,       0,  'h' },
    {0,                       0,                 0,  0   }
  };

  int long_index =0;
  int opt= 0;
  while ((opt = getopt_long(argc, argv,"c:C:f:h",long_options, &long_index )) != -1)
    {
      switch(opt)
	{
	case 'c' : CaloHitCollectionNames.push_back(optarg);
	  while (optind<argc && argv[optind][0] != '-') { CaloHitCollectionNames.push_back(argv[optind]); ++optind;}
	  break;
	case 'C' :ClusterCollectionNames.push_back(optarg);
	  while (optind<argc && argv[optind][0] != '-') { ClusterCollectionNames.push_back(argv[optind]); ++optind;}
	  break;
	case 'f' : outputFileName=optarg; break;
	case 'h' : usage(argv[0]); return 0;
	default  : usage(argv[0]); return 1;
	}
    }

  
  std::cout << "CaloHit collections: ";
  for (auto &m : CaloHitCollectionNames) std::cout << m << " ";
  std::cout << std::endl;
  std::cout << "Cluster collections: ";
  for (auto &m : ClusterCollectionNames) std::cout << m << " ";
  std::cout << std::endl;


  std::cout << "The program is not yet written" << std::endl;
  /*
  LCWriter* lcWrt = LCFactory::getInstance()->createLCWriter();
  lcWrt->open(outputFileName.c_str(), LCIO::WRITE_NEW );

  LCEventImpl* evt=createEvent(nombreDeHits,CaloHitCollectionNames,ClusterCollectionNames);
  lcWrt->writeEvent( evt );
  delete evt;

  lcWrt->close();
  delete lcWrt;
  */
  
  return 0;
};
