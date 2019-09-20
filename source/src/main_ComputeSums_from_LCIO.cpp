#include "HitClusterInfo_LCIO.h"

#include <iostream>
#include <fstream>
#include <getopt.h>

#include "IO/LCReader.h"


void usage(char* progname)
{
   std::cout << "Usage :" << progname << " takes the following options : " << std::endl;
   std::cout << "\t -c X or --calohit_col_name : white space separated list of LCIO CalorimeterHit collections names. (default is none)" << std::endl;
   std::cout << "\t -C X or --cluster_col_name  : white space separated list of LCIO Cluster collections names. (default is none)" << std::endl;
   std::cout << "\t \t \t \t  You need to provide at last 2 Cluster collection names to runs clustering comparisons" << std::endl;
   std::cout << "\t -i or --output_file_name : white space separated list of input slcio file names. You should provide at least one. (no default)"  << std::endl;
   std::cout << "\t -f string or --output_file_name=string : the output file name. (default is ComputeSums.txt)"  << std::endl;
   std::cout << "\t -h or --help : print this help and exit"  << std::endl;
   std::cout << "Examples:" <<std::endl;
   std::cout << "\t" << progname << " -c hcolOne hcolTwo hcolThree -C ClusColOne ClusColTwo -i test.slcio" << std::endl;
   std::cout << "\t" << progname << " -c hcolOne -C ClusColOne ClusColTwo -c hcolBisOne hcolBisTwo -i test.slcio" << std::endl;
}


int main(int argc, char **argv)
{
  // parameters
  std::vector<std::string> CaloHitCollectionNames;
  std::vector<std::string> ClusterCollectionNames;
  std::vector<std::string> InputFileNames;
  std::string outputFileName="ComputeSums.txt";
  
  //option decoding
  static struct option long_options[] = {
    {"calohit_col_name",      required_argument, 0,  'c' },
    {"cluster_col_name",      required_argument, 0,  'C' },
    {"input_file_name",       required_argument, 0,  'i' },
    {"output_file_name",      required_argument, 0,  'f' },
    {"help",                  no_argument,       0,  'h' },
    {0,                       0,                 0,  0   }
  };

  int long_index =0;
  int opt= 0;
  while ((opt = getopt_long(argc, argv,"c:C:i:f:h",long_options, &long_index )) != -1)
    {
      switch(opt)
	{
	case 'c' : CaloHitCollectionNames.push_back(optarg);
	  while (optind<argc && argv[optind][0] != '-') { CaloHitCollectionNames.push_back(argv[optind]); ++optind;}
	  break;
	case 'C' :ClusterCollectionNames.push_back(optarg);
	  while (optind<argc && argv[optind][0] != '-') { ClusterCollectionNames.push_back(argv[optind]); ++optind;}
	  break;
	case 'i' :InputFileNames.push_back(optarg);
	  while (optind<argc && argv[optind][0] != '-') { InputFileNames.push_back(argv[optind]); ++optind;}
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
  std::cout << "Input files: ";
  for (auto &m : InputFileNames) std::cout << m << " ";
  std::cout << std::endl;

  if (InputFileNames.empty())
    {
      std::cerr << "You should provide at least one input file" << std::endl;
      usage(argv[0]);
      return 1;
    }
  if (ClusterCollectionNames.size()<2)
    {
      std::cerr << "You should provide at least two cluster collection names" << std::endl;
      usage(argv[0]);
      return 1;
    }

  HitClusterInfo_LCIO HCI_lcio(CaloHitCollectionNames,ClusterCollectionNames);
  LCEvent *evt=nullptr;
  std::ofstream outputFile;
  outputFile.open(outputFileName.c_str());
  
  LCReader* lcReader = LCFactory::getInstance()->createLCReader();
  if (CaloHitCollectionNames.size()>0)
    {
      std::vector<std::string> collectionsToRead=CaloHitCollectionNames;
      collectionsToRead.insert(collectionsToRead.end(),ClusterCollectionNames.begin(),ClusterCollectionNames.end());
      lcReader->setReadCollectionNames(collectionsToRead);
    }
  
  for (auto& inputfilename :  InputFileNames)
    {
      try
	{
	  lcReader->open(inputfilename);
	  outputFile << "file " << inputfilename << " : " << std::endl;
	  while (evt=lcReader->readNextEvent())
	    {
	      outputFile << "event " << evt->getEventNumber() << " in run " << evt->getRunNumber() << " : " << std::endl;
	      //processing to write
	      const HitClusterInfo& HCI=HCI_lcio.analyseEvent(evt);
	      outputFile << HCI.numberOfHits() << " hits: nclusters=";
	      std::vector<unsigned int> clusterInfo=HCI.numberOfClustersPerClustering();
	      for (unsigned int ic=0; ic<ClusterCollectionNames.size(); ++ic)
		{
		  outputFile << "( " <<  ClusterCollectionNames[ic] << " " <<  clusterInfo[ic] << " )";
		}
	      outputFile << std::endl;
	      std::map<HitClusterInfo::ClusterSetIndices,ClusterPairsDataSums> m=HCI.getAllDataSums();
	      for (unsigned  int ic=0; ic<ClusterCollectionNames.size(); ++ic)
		for (unsigned int icbis=ic+1; icbis<ClusterCollectionNames.size(); ++icbis)
		  {
		    const ClusterPairsDataSums& CP=m[std::make_pair(ic,icbis)];
		    outputFile << "\t" << ClusterCollectionNames[ic] << " vs " << ClusterCollectionNames[icbis] << " : " << CP;
		    outputFile << " || Rand Index=" << CP.RandIndex();
		    outputFile << " || Jacard Index=" << CP.JacardIndex();
		    outputFile << " || Dice Index=" << CP. DiceIndex();
		    outputFile << " || Fowlkes Mallows Index=" << CP.FowlkesMallowsIndex();
		    outputFile <<  std::endl;
		  }
	    }
	  lcReader->close();
	}
      catch(IOException& e)
	{
	  std::cout << "WARNING WARNING WARNING: " << e.what() << " : skipping it.  WARNING WARNING WARNING" << std::endl;
	}
    }

  outputFile.close();
  return 0;
};
