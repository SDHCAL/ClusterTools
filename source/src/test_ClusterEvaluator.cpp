#include "ClusterEvaluator.h"

#include <assert.h>
#include <stdexcept>

int main()
{
  HitClusterInfo a;
  assert(a.containerSize()==0);
  assert(a.checkHitUnicity());
  HitClusterInfo b(4);
  assert(b.containerSize()==0);
  HitClusterInfo c(2,500);
  assert(c.containerSize()==1500);
  assert(! c.checkHitUnicity());
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
  assert(a.checkHitUnicity());
  a.addHit(myCharPointer);
  assert(a.containerSize()==6);
  assert(! a.checkHitUnicity());
  
  bool addOK=false;
  addOK=b.addHitCheck(myCharPointer);
  assert(b.containerSize()==5);
  assert(addOK==true);
  assert(b.checkHitUnicity());
  addOK=b.addHitCheck(myCharPointer);
  assert(b.containerSize()==5);
  assert(addOK==false);
  assert(b.checkHitUnicity());

  c.addHitCheck(myCharPointer);
  assert(c.containerSize()==1503);

  return 0;
}
