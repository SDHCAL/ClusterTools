//
//  This file is included in the library only if the library is compiled with ROOT dictionnary generation.
//
#include "ClusterPairsDataSums.h"

#include <fstream>

#include "TTree.h"

TTree* ClusterPairsDataSums_ASCIItoTTree(const char *inputfilename, const char* treeName)
{
  ClusterPairsDataSums data;

  TTree *tree=new TTree(treeName,"ClusterPairsDataSums to compute clustering distance");
  auto branch = tree->Branch("data", &data, 32000, 0);

  std::ifstream inputfile;
  inputfile.open(inputfilename);

  while (1)
    {
      inputfile >> data;
      if (!inputfile.good()) break;
      tree->Fill();
    }

  inputfile.close();
  return tree;
}
