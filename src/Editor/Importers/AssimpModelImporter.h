#ifndef EDITOR_IMPORTERS_ASSIMP_MODEL_IMPORTER_H_
#define EDITOR_IMPORTERS_ASSIMP_MODEL_IMPORTER_H_

#include "BaseModelImporter.h"
#include "assimp/scene.h"

namespace DreamEngine::Editor::Importers
{
class AssimpModelImporter : public BaseModelImporter
{
   public:
    Model& Import(std::string const& path, bool gamma = false) override;
   private:
    Model m_model;
    std::string m_directory;
    std::string m_path;
    std::vector<std::string> m_texturePaths;
    void ProcessNode(aiNode* node, const aiScene* scene);
    Mesh* ProcessMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture*> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType textureType);
};

}  // namespace DreamEngine::Editor::Importers
#endif