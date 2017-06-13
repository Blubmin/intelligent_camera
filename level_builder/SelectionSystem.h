#pragma once

#include <GL\glew.h>

class Entity;
class ICamera;
class IRenderer;
class Scene;

class SelectionSystem {

    IRenderer* _child;
    GLuint _vao;
    GLuint _fbo;
    GLuint _tex;

    Entity* _selected;
public:
    SelectionSystem();
    ~SelectionSystem();

    virtual void update(float time_elapsed, Scene* scene, ICamera* camera);
};

