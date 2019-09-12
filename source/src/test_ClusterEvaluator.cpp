#include "ClusterEvaluator.h"

#include <assert.h>
#include <stdexcept>

int main()
{
  HitClusterInfo a;
  HitClusterInfo b(4);
  HitClusterInfo c(2,500);
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
}
