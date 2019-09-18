

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

add_executable(test_ClusterEvaluator src/test_ClusterEvaluator.cpp src/HitClusterInfo.cpp)
add_test(test_ClusterEvaluator_runs test_ClusterEvaluator)
