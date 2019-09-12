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

enable_testing()

ADD_SUBDIRECTORY( ./source )

include_directories( ${${PROJECT_NAME}_include_dir} )

add_library( ${PROJECT_NAME} SHARED ${${PROJECT_NAME}_libsources})

add_executable( ${PROJECT_NAME}_exe ${${PROJECT_NAME}_exesource} )
target_link_libraries( ${PROJECT_NAME}_exe  ${PROJECT_NAME} )

install (TARGETS ${PROJECT_NAME} DESTINATION lib)
install (TARGETS ${PROJECT_NAME}_exe DESTINATION bin)
