#include "EntityFactory.h"

#include "Model.h"
#include "Position.h"
#include "Rotation.h"
#include "Scale.h"

Entity* EntityFactory::create_entity(std::string model_id, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale) {
    Entity* e = new Entity(model_id);

    Model* c_model = ModelLoader::getModelByName(model_id);
    Position* c_pos = new Position(pos.x, pos.y, pos.z);
    Rotation* c_rot = new Rotation(rot.x, rot.y, rot.z);
    Scale* c_scale = new Scale(scale.x, scale.y, scale.z);

    e->addComponent(c_model);
    e->addComponent(c_pos);
    e->addComponent(c_rot);
    e->addComponent(c_scale);

    return e;
}
