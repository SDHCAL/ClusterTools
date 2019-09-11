#include "ClusterEvaluator.h"

int main()
{
  HitClusterInfo a;
  HitClusterInfo b(4);
  HitClusterInfo c(2,500);
  // This will crash
  HitClusterInfo d(1);
  return 0;
}
