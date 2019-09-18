

set (${PROJECT_NAME}_libsources
  ${CMAKE_CURRENT_SOURCE_DIR}/src/HitClusterInfo.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/ClusterPairsDataSums.cpp
  PARENT_SCOPE
  )

set (${PROJECT_NAME}_include_dir
  ${CMAKE_CURRENT_SOURCE_DIR}/include
  PARENT_SCOPE
  )

set ( ${PROJECT_NAME}_exesource 
  ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
  PARENT_SCOPE
  )

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(test_HitClusterInfo src/test_HitClusterInfo.cpp src/HitClusterInfo.cpp)
add_test(test_HitClusterInfo_runs test_HitClusterInfo)

if (BUILD_WITH_LCIO)
  add_executable(test_HitClusterInfo_LCIO src/test_HitClusterInfo_LCIO.cpp src/HitClusterInfo_LCIO.cpp)
  target_link_libraries(test_HitClusterInfo_LCIO ${LCIO_LIBRARIES})
  add_test(test_HitClusterInfo_LCIO_runs test_HitClusterInfo_LCIO)
endif()
