#include "Entity.h"

#include "Model.h"

using namespace std;

Entity::Entity() : Entity("") {

}

Entity::Entity(std::string tag) {
    _tag = tag;
    this->_mask = 0;
    this->components = map<unsigned long, Component*>();
}

Entity::~Entity()
{
    for (auto c : components) {
        if (dynamic_cast<Model*>(c.second)) continue;
        delete c.second;
    }
}

void Entity::addComponent(Component* component)
{
    _mask |= component->mask;
    this->components[component->mask] = component;
}

Component* Entity::getComponent(unsigned long mask)
{
    return this->components.at(mask);
}

bool Entity::check_mask(unsigned long mask) {
    return (this->_mask & mask) == mask;
}

std::string Entity::tag() {
    return _tag;
}
