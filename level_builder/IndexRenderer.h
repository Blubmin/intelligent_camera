#pragma once
#include <engine_base\IRenderer.h>

#include <glm\glm.hpp>

class Entity;
class ICamera;
class Program;
class Scene;

class IndexRenderer :
    public IRenderer {
    Program* _prog;
public:
    IndexRenderer();
    ~IndexRenderer();

    static glm::mat4 create_model_matrix(Entity* entity);
    void draw(Scene* scene, ICamera* cam);
};

