#################################################
# cmake file for building SDHCAL ClusterTools package
# @author GGérald Grenier ,IP2I Lyon
# IP2I = Institut de Physique des 2 infinis Lyon  
# Copyright (c) IP2I and its supervisory administrations
CMAKE_MINIMUM_REQUIRED( VERSION 2.8 FATAL_ERROR )
#################################################

# project name
PROJECT( ClusterTools )

SET( ${PROJECT_NAME}_VERSION_MAJOR 1 )
SET( ${PROJECT_NAME}_VERSION_MINOR 0 )
SET( ${PROJECT_NAME}_VERSION_PATCH 0 )

set (CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR})

add_definitions("-std=c++11")

option(BUILD_WITH_ROOT "Link to ROOT and generate some ROOT dictionnary" OFF)

enable_testing()

if (BUILD_WITH_ROOT)
  message ("BUILD_WITH_ROOT activated")
  # You need to tell CMake where to find the ROOT installation. This can be done in a number of ways:
  #   - ROOT built with classic configure/make use the provided $ROOTSYS/etc/cmake/FindROOT.cmake
  #   - ROOT built with CMake. Add in CMAKE_PREFIX_PATH the installation prefix for ROOT
  list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})
  #---Locate the ROOT package and defines a number of variables (e.g. ROOT_INCLUDE_DIRS)
  find_package(ROOT REQUIRED)
  #---Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
  include(${ROOT_USE_FILE})
endif()


ADD_SUBDIRECTORY( ./source )

include_directories( ${${PROJECT_NAME}_include_dir} )

if (BUILD_WITH_ROOT)
  ROOT_GENERATE_DICTIONARY(G__ClusterPairsDataSums ClusterPairsDataSums.h LINKDEF ${${PROJECT_NAME}_include_dir}/ClusterPairsDataSumsLinkDef.h)
  set(ROOT_DICT_SOURCE G__ClusterPairsDataSums.cxx)
endif()

add_library( ${PROJECT_NAME} SHARED ${${PROJECT_NAME}_libsources} ${ROOT_DICT_SOURCE})

add_executable( ${PROJECT_NAME}_exe ${${PROJECT_NAME}_exesource} )
target_link_libraries( ${PROJECT_NAME}_exe  ${PROJECT_NAME} )
if (BUILD_WITH_ROOT)
  target_link_libraries( ${PROJECT_NAME}_exe ${ROOT_LIBRARIES} )
endif()

install (TARGETS ${PROJECT_NAME} DESTINATION lib)
install (TARGETS ${PROJECT_NAME}_exe DESTINATION bin)
