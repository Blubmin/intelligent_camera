#pragma once
#include <engine_base\IRenderer.h>

#include <glm\glm.hpp>

class Entity;
class ICamera;
class Program;
class Scene;

class SelectionRenderer :
    public IRenderer {

    Program* _prog;
    Program* _prog_outline;

public:
    SelectionRenderer();
    ~SelectionRenderer();
    glm::mat4 create_model_matrix(Entity* entity);
    glm::mat4 create_outline_model_matrix(Entity* entity);
    void draw(Scene* scene, ICamera* cam);
};

