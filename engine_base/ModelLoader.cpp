#include "ModelLoader.h"

#include <filesystem>
#include <vector>

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
#include <glm\glm.hpp>

using namespace std;
using namespace glm;

namespace fs = std::experimental::filesystem;

ModelLoader* ModelLoader::_instance = nullptr;

ModelLoader::ModelLoader() : ModelLoader("") {
}

ModelLoader::ModelLoader(std::string asset_folder) {
    _folder = asset_folder;

    _extensions.push_back(".obj");
}

ModelLoader::~ModelLoader()
{

}

void ModelLoader::init(const std::string& asset_folder) {
    if (_instance == nullptr) _instance = new ModelLoader(asset_folder);
    else _instance->_folder = asset_folder;
}

Model* ModelLoader::loadModelByName(const string& filename, string key)
{
    if (_instance == nullptr) _instance = new ModelLoader("");

    Assimp::Importer importer;

    const aiScene* scene = importer.ReadFile(filename,
        aiProcess_CalcTangentSpace |
        aiProcess_Triangulate |
        aiProcess_JoinIdenticalVertices |
        aiProcess_GenSmoothNormals
        );
     
    Model* model = new Model(scene);
    _instance->models[key] = model;
    return model;
}

Model* ModelLoader::getModelByName(string key)
{
    if (_instance == nullptr) _instance = new ModelLoader("");

    if (_instance->models.find(key) == _instance->models.end()) {
        auto begin = fs::begin(fs::directory_iterator(_instance->_folder));
        auto end = fs::end(fs::directory_iterator(_instance->_folder));
      
        for (auto& f : fs::directory_iterator(_instance->_folder)) {
            for (auto e : _instance->_extensions) {
                if (f != fs::path(_instance->_folder + "/" + key + e)) continue;

                loadModelByName(f.path().string(), key);
                return _instance->models.at(key);
            }
        }
        return nullptr;
    }
    return _instance->models.at(key);
}

std::vector<std::string> ModelLoader::keys() {
    vector<string> keys;
    for (auto p : _instance->models) {
        keys.push_back(p.first);
    }
    return keys;
}
