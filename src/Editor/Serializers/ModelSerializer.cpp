#include "ModelSerializer.h"
#include "Application.h"
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace DreamEngine::Editor::Serializers;

std::string ModelSerializer::Serialize(Model& model)
{
    json j;

    for (Mesh* mesh : model.meshes)
    {
        json jMesh;
        jMesh["Name"] = mesh->name;
        jMesh["ResourceId"] = mesh->resourceId;

        // Serialize indices
        jMesh["Indices"] = mesh->indices;

        // Serialize vertices
        for (Vertex& vertex : mesh->vertices)
        {
            json jVertex;
            jVertex["Position"] = {{"X", vertex.position.x}, {"Y", vertex.position.y}, {"Z", vertex.position.z}};
            jVertex["Normal"] = {{"X", vertex.normal.x}, {"Y", vertex.normal.y}, {"Z", vertex.normal.z}};
            jVertex["TexCoords"] = {{"X", vertex.texCoords.x}, {"Y", vertex.texCoords.y}};
            jMesh["Vertices"].push_back(jVertex);
        }

        // Serialize textures
        if (mesh->textures.empty())
            jMesh["Textures"] = json::array();

        for (Texture* texture : mesh->textures)
        {
            json jTexture;
            jTexture["ResourceId"] = texture->resourceId;
            jTexture["Name"] = texture->name;
            jTexture["Type"] = texture->type;
            jTexture["Path"] = texture->path;
            jMesh["Textures"].push_back(jTexture);
        }

        j["Meshes"].push_back(jMesh);
    }

    return j.dump(4);  // Pretty print with 4 spaces
}

Model& ModelSerializer::Deserialize(std::ifstream& stream)
{
    json j;
    stream >> j;

    Model* model = new Model();

    for (const auto& jMesh : j["Meshes"])
    {
        Mesh* mesh = Core::Application::Instance().GetRenderAPI()->CreateMesh({}, {}, {});
        mesh->name = jMesh["Name"];
        mesh->resourceId = jMesh["ResourceId"];

        // Deserialize indices
        for (const auto& index : jMesh["Indices"])
        {
            mesh->indices.push_back(index);
        }

        // Deserialize vertices
        for (const auto& jVertex : jMesh["Vertices"])
        {
            Vertex vertex;
            vertex.position.x = jVertex["Position"]["X"];
            vertex.position.y = jVertex["Position"]["Y"];
            vertex.position.z = jVertex["Position"]["Z"];
            vertex.normal.x = jVertex["Normal"]["X"];
            vertex.normal.y = jVertex["Normal"]["Y"];
            vertex.normal.z = jVertex["Normal"]["Z"];
            vertex.texCoords.x = jVertex["TexCoords"]["X"];
            vertex.texCoords.y = jVertex["TexCoords"]["Y"];
            mesh->vertices.push_back(vertex);
        }

        // Deserialize textures
        for (const auto& jTexture : jMesh["Textures"])
        {
            Texture* texture = new Texture(nullptr);
            texture->resourceId = jTexture["ResourceId"];
            texture->name = jTexture["Name"];
            texture->type = jTexture["Type"];
            texture->path = jTexture["Path"];
            mesh->textures.push_back(texture);
        }

        model->meshes.push_back(mesh);
    }

    return *model;
}