#include "HitClusterInfo.h"

#include <stdexcept>
#include <set>
#include <algorithm>



HitClusterInfo::HitClusterInfo(unsigned int nClusters,unsigned int nhits) :
  m_numberOfClusteringCases(nClusters),m_skip(nClusters+1),
  m_pointersToHits_and_Clusters((1+nClusters)*nhits,nullptr)
{
  if (nClusters<2) throw std::domain_error(" HitClusterInfo construction : minimum number of clusters is 2");
  rewind_iterator_memory_for_set_hit();
}


std::vector<const void *>::const_iterator HitClusterInfo::find(const void * hit) const
{
  for (std::vector<const void *>::const_iterator it=m_pointersToHits_and_Clusters.begin();
       it < m_pointersToHits_and_Clusters.end();
       it+=m_skip)
    if ( (*it)==hit) return it;
  return m_pointersToHits_and_Clusters.end();
}

// I know, it's duplicated code but I haven't figured out how to avoid it in proper C++ coding
std::vector<const void *>::iterator HitClusterInfo::find(const void * hit) 
{
  for (std::vector<const void *>::iterator it=m_pointersToHits_and_Clusters.begin();
       it < m_pointersToHits_and_Clusters.end();
       it+=m_skip)
    if ( (*it)==hit) return it;
  return m_pointersToHits_and_Clusters.end();
}

bool HitClusterInfo::checkHitUnicity() const
{
  std::set<const void *> hitSet;
  for (std::vector<const void *>::const_iterator it=m_pointersToHits_and_Clusters.begin();
       it < m_pointersToHits_and_Clusters.end();
       it+=m_skip)
    hitSet.insert(*it);
  return hitSet.size()==numberOfHits();
}


bool HitClusterInfo::addCluster(unsigned int partitionNumber,const void *hit, const void *clusterPointer,  bool addTheHit)
{
  std::vector<const void *>::iterator it=find(hit);
  bool hitfound=(it!=m_pointersToHits_and_Clusters.end());
  if (! hitfound)
    {
      if (addTheHit) {addHit(hit); it=find(hit);}
      else throw std::domain_error("HitClusterInfo::addCluster, hit not found");
    }
  *(it+(1+partitionNumber))=clusterPointer;
  return hitfound;
}


ClusterPairsDataSums HitClusterInfo::getDataSums(unsigned int firstClusterSetIndex,unsigned int secondClusterSetIndex) const
{
  //std::cout << "printout of " << m_pointersToHits_and_Clusters.size() << " hits and clusters : ";
  //for (auto p : m_pointersToHits_and_Clusters) std::cout << p << " ";
  //std::cout<<std::endl;
  //
  ClusterPairsDataSums result;
  unsigned int firstCluster=1+firstClusterSetIndex;
  unsigned int secondCluster=1+secondClusterSetIndex;
  const void* const* pbegin=m_pointersToHits_and_Clusters.data();
  const void* const* pend=pbegin+m_pointersToHits_and_Clusters.size();
  for (const void* const* p1=pbegin; p1<pend; p1+=m_skip)
    for (const void* const* p2=p1+m_skip; p2<pend; p2+=m_skip)
      if (*(p1+firstCluster)==*(p2+firstCluster))
	{
	  if (*(p1+secondCluster)==*(p2+secondCluster)) ++result.m_numberOfCoGroupedHitPair_InBothClusterSets;
	  else ++result.m_numberOfCoGroupedHitPair_OnlyInFirstClusterSet;
	}
      else
	{
	  if (*(p1+secondCluster)==*(p2+secondCluster)) ++result.m_numberOfCoGroupedHitPair_OnlyInSedondClusterSet;
	  else ++result.m_numberOfNonCoGroupedHitPair_InBothClusterSets;
	}
  return result;
}

std::map<HitClusterInfo::ClusterSetIndices,ClusterPairsDataSums> HitClusterInfo::getAllDataSums() const
{
  std::map<ClusterSetIndices,ClusterPairsDataSums> m;
  for (unsigned int i=0; i<m_numberOfClusteringCases; ++i)
    for (unsigned int j=i+1; j<m_numberOfClusteringCases; ++j)
      m[std::make_pair(i,j)]=getDataSums(i,j);
  return m;
}


std::vector<unsigned int> HitClusterInfo::numberOfClustersPerClustering() const
{
  std::set<const void *> computeClusters[m_numberOfClusteringCases];
  
  const void* const* pbegin=m_pointersToHits_and_Clusters.data();
  const void* const* pend=pbegin+m_pointersToHits_and_Clusters.size();
  for (const void* const* p=pbegin+1; p<pend; p+=m_skip)
    for (unsigned int icluster=0; icluster<m_numberOfClusteringCases; ++icluster)
      computeClusters[icluster].insert(*(p+icluster));
    
  std::vector<unsigned int> nClusters(m_numberOfClusteringCases,0);
  for (unsigned int icluster=0; icluster<m_numberOfClusteringCases; ++icluster)
    nClusters[icluster]=computeClusters[icluster].size();
  return nClusters;
}

void HitClusterInfo::randomClustering(std::vector<unsigned int> numbersOfClusters,bool exactClusterCount)
{
  unsigned int maxClusters=0;
  for (auto nClusters : numbersOfClusters)
    {
      if (nClusters>numberOfHits())
	throw std::domain_error(std::string("HitClusterInfo::randomClustering error : can't generate ")+std::to_string(nClusters)+std::string(" clusters from ")+std::to_string(numberOfHits())+std::string(" hits only."));
      if (nClusters>maxClusters) maxClusters=nClusters;
    }

  if (numbersOfClusters.size()<m_numberOfClusteringCases)
    numbersOfClusters.resize(m_numberOfClusteringCases,*numbersOfClusters.rbegin());

  double interval_separator[maxClusters];
  const void** pbegin=m_pointersToHits_and_Clusters.data();
  const void** pend=pbegin+m_pointersToHits_and_Clusters.size();

  for (unsigned int indexClustering=0; indexClustering<m_numberOfClusteringCases; ++indexClustering)
    {
      //random generation of the mean cluster fraction of hits
      unsigned int nClusters=numbersOfClusters[indexClustering];
      for (unsigned indexseparator=0;indexseparator<nClusters-1;++indexseparator)
	interval_separator[indexseparator]=drand48();
      std::sort(interval_separator,interval_separator+(nClusters-1));
      interval_separator[nClusters-1]=1.0;
      //generate random clusters
      const void** pCluster=pbegin+(indexClustering+1);
      if (exactClusterCount) //make sure each genertaed cluster will have at least one hit
	for (unsigned int indexCluster=0; indexCluster<nClusters; ++indexCluster)
	  {
	    (*pCluster)=interval_separator+indexCluster;
	    pCluster+=m_skip;
	  }
      for (; pCluster<pend; pCluster+=m_skip)
	{
	  double tir=drand48();
	  for (unsigned indexseparator=0;indexseparator<nClusters;++indexseparator)
	    if (tir<interval_separator[indexseparator])
	      {
		(*pCluster)=interval_separator+indexseparator;
		break;
	      }
	}
      //if exactcluster, shuffle the first nclusters hits
      pCluster=pbegin+(indexClustering+1);
      const void** pfirstHit=pCluster;
      if (exactClusterCount)
	if (nClusters<numberOfHits()) //nothing to swap do if nClusters==numberOfHits()
	    for (unsigned int indexCluster=0; indexCluster<nClusters; ++indexCluster)
	      {
		unsigned long int newplace = nClusters+(lrand48()%(numberOfHits()-nClusters));
		std::swap(*pCluster,*(pfirstHit+(newplace*m_skip)));
		pCluster+=m_skip;
	      }
    }
}


void HitClusterInfo_pairIterator::reset()
{
  pbegin=m_hci.m_pointersToHits_and_Clusters.data();
  pend=pbegin+m_hci.m_pointersToHits_and_Clusters.size();
  p.first=pbegin;
  p.second=p.first;
}


#ifdef BUILD_WITH_ROOT
HitClusterInfo_ToTtree::HitClusterInfo_ToTtree(const HitClusterInfo& hitclusterinfo,std::string treeName) :
  m_hitclusterinfo(hitclusterinfo)
{
  m_tree=new TTree(treeName.c_str(),"HitClusterInfo to TTree");
  unsigned int nClustering=hitclusterinfo.numberOfClusteringCases();
  m_treeIntData = new unsigned int[1+nClustering];
  m_treeDataSums = new ClusterPairsDataSums[nClustering*nClustering]; //It's more than twice too much space but is easier to compute index
  std::string ClusterInfoLeaf("NumberOfHits/i");
  for (unsigned int i=0; i<nClustering; ++i)
    ClusterInfoLeaf+=(std::string(":NumberOfClusters_")+std::to_string(i));
  m_tree->Branch("ClusterInfo",m_treeIntData,ClusterInfoLeaf.c_str());
  for (unsigned int ic=0; ic<nClustering; ++ic)
    for (unsigned int icbis=ic+1; icbis<nClustering; ++icbis)
      {
	std::string branchName("Sum_Clusters_");
	branchName+=std::to_string(ic)+std::string("_vs_")+std::to_string(icbis);
	m_tree->Branch(branchName.c_str(),m_treeDataSums+(nClustering*ic+icbis),32000,0);
      }
}

HitClusterInfo_ToTtree::~HitClusterInfo_ToTtree()
{
  delete [] m_treeDataSums;
  delete [] m_treeIntData;
  //delete m_tree; //ownership is transfered to TFile (ROOT up to 6)
}

void HitClusterInfo_ToTtree::Fill()
{
  m_treeIntData[0]=m_hitclusterinfo.numberOfHits();
  std::vector<unsigned int> nclus(m_hitclusterinfo.numberOfClustersPerClustering());
  std::copy(nclus.begin(),nclus.end(),m_treeIntData+1);
  std::map<HitClusterInfo::ClusterSetIndices,ClusterPairsDataSums> m=m_hitclusterinfo.getAllDataSums();
  unsigned int nClustering=m_hitclusterinfo.numberOfClusteringCases();  
  for (unsigned int ic=0; ic<nClustering; ++ic)
    for (unsigned int icbis=ic+1; icbis<nClustering; ++icbis)
      {
	ClusterPairsDataSums& CPDS=m[std::make_pair(ic,icbis)];
	m_treeDataSums[nClustering*ic+icbis]=CPDS;
      }
  m_tree->Fill();
}
#endif
