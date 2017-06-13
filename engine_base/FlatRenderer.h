#pragma once
#include "IRenderer.h"

#include <glm\glm.hpp>

class Entity;
class ICamera;
class Program;
class Scene;

class FlatRenderer :
    public IRenderer {

    Program* _prog;

public:
    FlatRenderer();
    ~FlatRenderer();

    static glm::mat4 create_model_matrix(Entity * entity);
    void draw(Scene* Scene, ICamera* cam);
};

