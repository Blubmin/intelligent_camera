#pragma once
#include "IRenderer.h"

#include <GL\glew.h>
#include <glm\glm.hpp>

class ICamera;
class Program;
class Scene;

class DebugRenderer :
    public IRenderer {

    Program* _prog;

    IRenderer* _child;
    GLuint _vao;
    GLuint _fbo;
    GLuint _tex;

    glm::vec2 _pos;
    float _scale;
public:
    DebugRenderer();
    DebugRenderer(IRenderer* renderer, glm::vec2, float scale);
    ~DebugRenderer();

    void draw(Scene* scene, ICamera* camera);
    GLuint texture();
};