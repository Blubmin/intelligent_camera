add_library(engine_base
  BasicCamera.h
  BasicCamera.cpp
  Component.h
  Component.cpp
  DebugRenderer.h
  DebugRenderer.cpp
  Entity.h
  Entity.cpp
  EntityFactory.h
  EntityFactory.cpp
  ExceptionWrapper.h
  ExceptionWrapper.cpp
  FlatRenderer.h
  FlatRenderer.cpp
  GLSL.h
  GLSL.cpp
  ICamera.h
  ICamera.cpp
  imgui_impl_glfw_gl3.h
  imgui_impl_glfw_gl3.cpp
  Input.h
  Input.cpp
  IRenderer.h
  ISystem.h
  Material.h
  Material.cpp
  Mesh.h
  Mesh.cpp
  Model.h
  Model.cpp
  ModelLoader.h
  ModelLoader.cpp
  Position.h
  Position.cpp
  Program.h
  Program.cpp
  Rotation.h
  Rotation.cpp
  Scale.h
  Scale.cpp
  Scene.h
  Scene.cpp
  StaticCamera.h
  StaticCamera.cpp
  TextureRenderer.h
  TextureRenderer.cpp
  Utility.h
)

target_link_libraries(engine_base
  # ${assimp}
  # ${glew_LIBRARY}
  imgui
)

# add_dependencies(engine_base
# project_assimp
#   glm
# )