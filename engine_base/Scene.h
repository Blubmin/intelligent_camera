#pragma once

#include <string>
#include <vector>

#include <glm\glm.hpp>

class Entity;
class ICamera;
class IRenderer;
class ISystem;

class Scene {
    std::vector<Entity*> _entites;
    std::vector<ISystem*> _systems;
    IRenderer* _renderer;
    glm::vec3 _dir_light;
public:
    Scene();
    ~Scene();
    void set_renderer(IRenderer* renderer);
    void add_system(ISystem* system);
    void add_entity(Entity* entity);
    std::vector<Entity*>& entities();
    glm::vec3 dir_light();
    void update(float time_elapsed);
    void draw(ICamera* cam);
    void save(const std::string& filename);
};

