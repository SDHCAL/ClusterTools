# ClusterTools
Tools to evaluate SDHCAL software clustering quality and compute cluster properties.


## License
Code licensed under the CeCILL 2.1 license :
- http://www.cecill.info/licences/Licence_CeCILL_V2.1-fr.txt
- http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt (a license copy is provided in the COPYING.txt file)

## Installation

- mkdir build
- cd build 
- cmake ..
- optional : make test 
- make install

### Installation with ROOT
If you have ROOT installed, you can replace
`cmake ..`
by
`cmake -D BUILD_WITH_ROOT=ON ..`

In that case, if your LD_LIBRARY_PATH points to the lib directory,
you can convert the ASCII output of the ClusterTools_exe executable to a ROOT TTree.
At the ROOT prompter do
```C++
.L ../lib/libClusterTools.so
TTree dummy // to get the TTree shared library loaded
TTree *t=ClusterPairsDataSums_ASCIItoTTree("random_test.txt")
```
and then, you can do
```C++
t->Draw("data.a()")
t->Draw("data.RandIndex()")
```

### Installation with LCIO
If you have LCIO installed, you can replace
`cmake ..`
by
`cmake -D BUILD_WITH_LCIO=ON ..`

The installation with LCIO allows to use LCIO files with LCIO::EVENT::CalorimeterHit
and LCIO::EVENT::Cluster as input to compute sums to estimate cluster set differences
like Rand index.

2 executables are built by the LCIO build option :

ComputeSums_from_LCIO to Compute the sums and ClusterTools_LCIOwrite_test to write a
LCIO file that can be used as input to test the behaviour of ComputeSums_from_LCIO

### installation with LCIO and ROOT
In this case, the ComputeSums_from_LCIO executable will store the computed sum in ROOT trees.
