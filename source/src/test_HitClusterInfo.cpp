#include "HitClusterInfo.h"

#include <assert.h>
#include <stdexcept>
#include <initializer_list>

#include <iostream>

void checkValue(const ClusterPairsDataSums& p, unsigned int both, unsigned int firstOnly, unsigned int secondOnly, unsigned int none)
{
  std::cout << "a=" << p.a() << " b=" << p.b() << " c=" << p.c() << " d=" << p.d() << std::endl;
  assert (p.a()==both);
  assert (p.b()==none);
  assert (p.c()==firstOnly);
  assert (p.d()==secondOnly);
}


int main()
{
  bool exceptionThrown = false;
  try
    {
      HitClusterInfo dummy(1);
    }
  catch(std::domain_error&)
    {
      exceptionThrown = true;
    }
  assert(exceptionThrown);


  char myChar[1000];
  char* myCharPointer=myChar;
  std::vector<int> intvec(100,34);

  HitClusterInfo a;
  assert(a.numberOfHits()==0);
  assert(a.numberOfClustersPerClustering().size()==2);
  assert(a.numberOfClustersPerClustering()==std::vector<unsigned int>(std::initializer_list<unsigned int>({0,0})));
  assert(a.checkHitUnicity());
  checkValue(a.getDataSums(0,1),0,0,0,0);
  a.addHit(myCharPointer);
  assert(a.numberOfHits()==1);
  assert(a.numberOfClustersPerClustering().size()==2);
  assert(a.numberOfClustersPerClustering()==std::vector<unsigned int>(std::initializer_list<unsigned int>({1,1})));
  assert(a.checkHitUnicity());
  checkValue(a.getDataSums(0,1),0,0,0,0);
  a.addHit(myCharPointer);
  assert(a.numberOfHits()==2);
  assert(a.numberOfClustersPerClustering().size()==2);
  assert(a.numberOfClustersPerClustering()==std::vector<unsigned int>(std::initializer_list<unsigned int>({1,1})));
  assert(! a.checkHitUnicity());
  checkValue(a.getDataSums(0,1),1,0,0,0); //2 hits, only one cluster
  bool addClusterOK=a.addCluster(0,myCharPointer,intvec.data(),true);
  assert(addClusterOK);
  assert(a.numberOfHits()==2);
  assert(a.numberOfClustersPerClustering().size()==2);
  assert(a.numberOfClustersPerClustering()==std::vector<unsigned int>(std::initializer_list<unsigned int>({2,1})));
  checkValue(a.getDataSums(0,1),0,0,1,0); //2 hits, the second cluster set with one cluster, the first cluster set with 2 clusters
  addClusterOK=a.addCluster(0,myCharPointer+1,intvec.data(),true);
  assert(! addClusterOK);
  assert(a.numberOfHits()==3);
  assert(a.numberOfClustersPerClustering().size()==2);
  assert(a.numberOfClustersPerClustering()==std::vector<unsigned int>(std::initializer_list<unsigned int>({2,1})));
  checkValue(a.getDataSums(0,1),1,0,2,0); //3 hits, cluster set 1 = ({1,3},{2}), cluster set 2 = ({1,2,3})
  std::map<HitClusterInfo::ClusterSetIndices,ClusterPairsDataSums> m=a.getAllDataSums();
  assert(m.size()==1);

  HitClusterInfo b(4);
  assert(b.numberOfHits()==0);
  assert(b.numberOfClustersPerClustering().size()==4);
  assert(b.numberOfClustersPerClustering()==std::vector<unsigned int>(std::initializer_list<unsigned int>({0,0,0,0})));
  bool addOK=false;
  addOK=b.addHitCheck(myCharPointer);
  assert(b.numberOfHits()==1);
  assert(b.numberOfClustersPerClustering().size()==4);
  assert(b.numberOfClustersPerClustering()==std::vector<unsigned int>(std::initializer_list<unsigned int>({1,1,1,1})));
  assert(addOK==true);
  assert(b.checkHitUnicity());
  addOK=b.addHitCheck(myCharPointer);
  assert(b.numberOfHits()==1);
  assert(addOK==false);
  assert(b.checkHitUnicity());
  addClusterOK=b.addCluster(0,myCharPointer,intvec.data());
  assert(addClusterOK);
  assert(b.numberOfHits()==1);
  assert(b.numberOfClustersPerClustering()==std::vector<unsigned int>(std::initializer_list<unsigned int>({1,1,1,1})));
  exceptionThrown = false;
  try { b.addCluster(0,myCharPointer+1,intvec.data());}
  catch (std::domain_error&) {exceptionThrown=true;}
  assert(exceptionThrown);
  assert(b.numberOfHits()==1);
  assert(b.numberOfClustersPerClustering()==std::vector<unsigned int>(std::initializer_list<unsigned int>({1,1,1,1})));
  exceptionThrown = false;
  try { b.addClusterCheck(0,myCharPointer,intvec.data());}
  catch (std::range_error&) {exceptionThrown=true;}
  assert(exceptionThrown==false);
  assert(b.numberOfHits()==1);
  try { b.addClusterCheck(120,myCharPointer,intvec.data());}
  catch (std::range_error&) {exceptionThrown=true;}
  assert(exceptionThrown);
  assert(b.numberOfHits()==1);
  m=b.getAllDataSums();
  assert(m.size()==4*3/2);
  exceptionThrown = false;
  try { b.getDataSumsCheck(0,7);}
  catch (std::range_error&) {exceptionThrown=true;}
  assert(exceptionThrown);

  HitClusterInfo c(2,500);
  assert(c.numberOfHits()==500);
  assert(c.numberOfClustersPerClustering().size()==2);
  assert(c.numberOfClustersPerClustering()==std::vector<unsigned int>(std::initializer_list<unsigned int>({1,1})));
  assert(! c.checkHitUnicity());
  checkValue(c.getDataSums(0,1),500*499/2,0,0,0); //2 * 500 hits dans un meme cluster
  c.addHitCheck(myCharPointer);
  assert(c.numberOfHits()==501);
  assert(c.numberOfClustersPerClustering().size()==2);
  assert(c.numberOfClustersPerClustering()==std::vector<unsigned int>(std::initializer_list<unsigned int>({1,1})));
  assert(! c.checkHitUnicity());
  checkValue(c.getDataSums(0,1),501*500/2,0,0,0); //2 * 501 hits dans un meme cluster
  c.setHits(myCharPointer,myCharPointer+501);
  assert(c.checkHitUnicity());  
  c.setHits(intvec.begin(),intvec.end());
  assert(c.checkHitUnicity());
  c.setHits(intvec);
  assert(c.checkHitUnicity());
  c.addCluster(0,&(*intvec.begin()),intvec.data());
  assert(c.numberOfClustersPerClustering().size()==2);
  assert(c.numberOfClustersPerClustering()==std::vector<unsigned int>(std::initializer_list<unsigned int>({2,1})));
  checkValue(c.getDataSums(0,1),500*499/2,0,500,0); //2 clusters 1 hit and 500 hits versus 1 cluster of 501 hits

  c.reset(200);
  assert(c.numberOfHits()==200);
  assert(c.numberOfClustersPerClustering().size()==2);
  assert(c.numberOfClustersPerClustering()==std::vector<unsigned int>(std::initializer_list<unsigned int>({1,1})));
  assert(! c.checkHitUnicity());
  checkValue(c.getDataSums(0,1),200*199/2,0,0,0); //2 * 200 hits dans un meme cluster
  
  return 0;
}
