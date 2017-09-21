ExternalProject_Add(project_tfd
  GIT_REPOSITORY https://git.code.sf.net/p/tinyfiledialogs/code tinyfiledialogs
  UPDATE_COMMAND ""
)

add_library(tinyfiledialogs
  tinyfiledialogs.h
  tinyfiledialogs.c
)

add_dependencies(tinyfiledialogs
  project_tfd
)

include_directories(tinyfiledialogs)