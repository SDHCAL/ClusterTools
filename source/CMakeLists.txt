

set (${PROJECT_NAME}_libsources
  ${CMAKE_CURRENT_SOURCE_DIR}/src/ClusterEvaluator.cpp
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
