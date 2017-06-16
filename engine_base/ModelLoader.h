#pragma once

#include <map>
#include <memory>
#include <string>
#include <vector>

#include "Model.h"

class ModelLoader
{
    ModelLoader();
    ModelLoader(std::string asset_folder);
    ~ModelLoader();

    std::string _folder;
    std::map<std::string, Model*> models;
    std::vector<std::string> _extensions;

    static ModelLoader* _instance;

public:
    static void init(const std::string& asset_folder);
    static Model* loadModelByName(const std::string& filename, std::string key);
    static Model* getModelByName(std::string key);
    static std::vector<std::string> keys();
};

