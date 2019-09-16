#include "ClusterEvaluator.h"

#include <assert.h>
#include <stdexcept>

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
  assert(a.containerSize()==0);
  assert(a.checkHitUnicity());
  a.addHit(myCharPointer);
  assert(a.containerSize()==3);
  assert(a.checkHitUnicity());
  a.addHit(myCharPointer);
  assert(a.containerSize()==6);
  assert(! a.checkHitUnicity());
  bool addClusterOK=a.addCluster(0,myCharPointer,intvec.data(),true);
  assert(addClusterOK);
  assert(a.containerSize()==6);
  addClusterOK=a.addCluster(0,myCharPointer+1,intvec.data(),true);
  assert(! addClusterOK);
  assert(a.containerSize()==9);

  HitClusterInfo b(4);
  assert(b.containerSize()==0);
  bool addOK=false;
  addOK=b.addHitCheck(myCharPointer);
  assert(b.containerSize()==5);
  assert(addOK==true);
  assert(b.checkHitUnicity());
  addOK=b.addHitCheck(myCharPointer);
  assert(b.containerSize()==5);
  assert(addOK==false);
  assert(b.checkHitUnicity());
  addClusterOK=b.addCluster(0,myCharPointer,intvec.data());
  assert(addClusterOK);
  assert(b.containerSize()==5);
  exceptionThrown = false;
  try { b.addCluster(0,myCharPointer+1,intvec.data());}
  catch (std::domain_error&) {exceptionThrown=true;}
  assert(exceptionThrown);
  assert(b.containerSize()==5);
  exceptionThrown = false;
  try { b.addClusterCheck(0,myCharPointer,intvec.data());}
  catch (std::range_error&) {exceptionThrown=true;}
  assert(exceptionThrown==false);
  assert(b.containerSize()==5);
  try { b.addClusterCheck(120,myCharPointer,intvec.data());}
  catch (std::range_error&) {exceptionThrown=true;}
  assert(exceptionThrown);
  assert(b.containerSize()==5);
  

  HitClusterInfo c(2,500);
  assert(c.containerSize()==1500);
  assert(! c.checkHitUnicity());
  c.addHitCheck(myCharPointer);
  assert(c.containerSize()==1503);
  assert(! c.checkHitUnicity());
  c.setHits(myCharPointer,myCharPointer+501);
  assert(c.checkHitUnicity());  
  c.setHits(intvec.begin(),intvec.end());
  assert(c.checkHitUnicity());
  c.setHits(intvec);
  assert(c.checkHitUnicity());


  return 0;
}
