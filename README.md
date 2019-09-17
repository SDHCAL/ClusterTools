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
    cmake ..
by
    cmake -D BUILD_WITH_ROOT=ON ..

In that case, if your LD_LIBRARY_PATH points to the lib directory,
you can convert the ASCII output of the ClusterTools_exe executable to a ROOT TTree.
At the ROOT prompter do
    .L ../lib/libClusterTools.so
    TTree dummy // to get the TTree shared library loaded
    TTree *t=ClusterPairsDataSums_ASCIItoTTree("random_test.txt")
and then, you can do
    t->Draw("data.a()")
    t->Draw("data.RandIndex()")


