#include "ProjectConfigurationSerializer.h"

#include <nlohmann/json.hpp>

using namespace nlohmann;
using namespace DreamEngine::Editor::Serializers;

std::string ProjectConfigurationSerializer::Serialize(ProjectConfiguration& model)
{
    json j;
    j["projectName"] = model.projectName;
    j["projectPath"] = model.projectPath;
    j["csSolution"] = model.csSolution;
    j["csProjectPath"] = model.csProjectPath;
    j["csProjectDebugPath"] = model.csProjectDebugPath;
    j["csProjectDebugDll"] = model.csProjectDebugDll;
    j["sceneDefinitions"] = model.sceneDefinitions;

    return j.dump(4);  // Pretty print with 4 spaces
}

ProjectConfiguration& ProjectConfigurationSerializer::Deserialize(std::ifstream& stream)
{
    json j;
    stream >> j;

    ProjectConfiguration* model = new ProjectConfiguration();
    model->projectName = j["projectName"].get<std::string>();
    model->projectPath = j["projectPath"].get<std::string>();
    model->csSolution = j["csSolution"].get<std::string>();
    model->csProjectPath = j["csProjectPath"].get<std::string>();
    model->csProjectDebugPath = j["csProjectDebugPath"].get<std::string>();
    model->csProjectDebugDll = j["csProjectDebugDll"].get<std::string>();
    model->sceneDefinitions = j["sceneDefinitions"].get<std::map<std::string, std::string>>();

    return *model;
}