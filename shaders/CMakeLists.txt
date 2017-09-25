SET(SHADER_FILES
  debug.frag
  debug.vert
  flat.frag
  flat.vert
  grid.frag
  grid.vert
  outline.frag
  outline.vert
  phong.frag
  phong.vert
)

add_library(shaders
  shaders.cpp
  shaders.h
  ${SHADER_FILES}
)

FUNCTION(PREPEND var prefix)
SET(listVar "")
FOREACH(f ${ARGN})
   LIST(APPEND listVar "${prefix}/${f}")
ENDFOREACH(f)
SET(${var} "${listVar}" PARENT_SCOPE)
ENDFUNCTION(PREPEND)

PREPEND(SHADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/" ${SHADER_FILES})

source_group(shaders FILES ${SHADER_FILES})

FUNCTION(JOIN VALUES GLUE OUTPUT)
string (REGEX REPLACE "([^\\]|^);" "\\1${GLUE}" _TMP_STR "${VALUES}")
string (REGEX REPLACE "[\\](.)" "\\1" _TMP_STR "${_TMP_STR}") #fixes escaping
set (${OUTPUT} "${_TMP_STR}" PARENT_SCOPE)
ENDFUNCTION()

JOIN("${SHADER_FILES}" "," shader_string)

set(bin2c_cmdline
  -DOUTPUT_C=shaders.cpp
  -DOUTPUT_H=shaders.h
  -DINPUT_FILES=${shader_string}
  -P "${CMAKE_CURRENT_SOURCE_DIR}/bin2c.cmake"
)

add_custom_command(
  TARGET shaders
  COMMAND ${CMAKE_COMMAND} ARGS ${bin2c_cmdline}
  COMMENT "Running bin2c"
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  PRE_BUILD VERBATIM
)