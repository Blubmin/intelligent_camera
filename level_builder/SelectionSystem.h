#pragma once

#include <GL\glew.h>
#include <glm\glm.hpp>

class Entity;
class ICamera;
class TextureRenderer;
class Scene;

class SelectionSystem {

    TextureRenderer* _child;
    GLuint _vao;
    GLuint _fbo;
    GLuint _tex;

    Entity* _selected;

    glm::vec3 _pos;
    glm::vec3 _rot;
    glm::vec3 _scale;

public:
    SelectionSystem();
    ~SelectionSystem();

    virtual void update(float time_elapsed, Scene* scene, ICamera* camera);
    void select(Entity* entity);
    bool selected();
    glm::vec3* pos();
    glm::vec3* rot();
    glm::vec3* scale();
};

