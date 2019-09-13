#include "ClusterEvaluator.h"

#include <assert.h>
#include <stdexcept>

int main()
{
  HitClusterInfo a;
  assert(a.containerSize()==0);
  HitClusterInfo b(4);
  assert(b.containerSize()==0);
  HitClusterInfo c(2,500);
  assert(c.containerSize()==1500);
  bool exceptionThrown = false;
  try
    {
      HitClusterInfo d(1);
    }
  catch(std::domain_error&)
    {
      exceptionThrown = true;
    }
  assert(exceptionThrown);

  char myChar[1000];
  char* myCharPointer=myChar;
  a.addHit(myCharPointer);
  assert(a.containerSize()==3);
  a.addHit(myCharPointer);
  assert(a.containerSize()==6);
  
  bool addOK=false;
  addOK=b.addHitCheck(myCharPointer);
  assert(b.containerSize()==5);
  assert(addOK==true);
  addOK=b.addHitCheck(myCharPointer);
  assert(b.containerSize()==5);
  assert(addOK==false);

  c.addHitCheck(myCharPointer);
  assert(c.containerSize()==1503);

  return 0;
}
