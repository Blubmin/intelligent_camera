cmake_minimum_required(VERSION 3.6)
include(ExternalProject)

project(intelligent_camera
  VERSION 1.0.0
  LANGUAGES CXX
)

find_package(GLEW REQUIRED)
if (GLEW_FOUND)
  include_directories(${GLEW_INCLUDE_DIRS})

  # add_library(glew STATIC IMPORTED)
  # set_target_properties(glew
  #   PROPERTIES
  #     IMPORTED_LOCATION ${GLEW_LIBRARY}
  # )
  link_libraries(${GLEW_LIBRARIES})
endif()

find_package(assimp REQUIRED)
include_directories("/include")
# if (assimp_FOUND)
# message(STATUS "Assimp include dir: ${ASSIMP_LIBRARIES}")
#   include_directories(${ASSIMP_INCLUDE_DIRS})
#   link_libraries(${ASSIMP_LIBRARIES})
# endif()

# find_package(assimp REQUIRED)
# if (assimp_FOUND)
#   include_directories(${Assimp_INCLUDE_DIRS})
#   link_libraries(${Assimp_LIBRARY})
# endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(third_party)

add_subdirectory(third_party)
add_subdirectory(engine_base)