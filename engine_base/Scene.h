#pragma once

#include <set>
#include <string>
#include <vector>

#include <glm\glm.hpp>

#define JSON_BUFFER_SIZE 65536

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
    void import(const std::string& filename);
    std::set<std::string> keys();

    static char const * FilterPatterns[1];
};

