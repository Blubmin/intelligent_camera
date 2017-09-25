cmake_minimum_required(VERSION 3.6)
include(ExternalProject)

project(intelligent_camera
  VERSION 1.0.0
  LANGUAGES C CXX
)

# find_package(GLEW REQUIRED)
# if (GLEW_FOUND)
#   include_directories(${GLEW_INCLUDE_DIRS})

#   add_library(glew STATIC IMPORTED)
#   set_target_properties(glew
#     PROPERTIES
#       IMPORTED_LOCATION ${GLEW_LIBRARIES}
#   )
# endif()

find_package(assimp REQUIRED) # target link assimp
find_package(gflags REQUIRED)
find_package(glfw3 REQUIRED) # target link glfw
find_package(glog REQUIRED) # target link glog::glog

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(third_party)

add_subdirectory(third_party)
add_subdirectory(engine_base)
add_subdirectory(intelligent_camera)
add_subdirectory(level_builder)
add_subdirectory(shaders)