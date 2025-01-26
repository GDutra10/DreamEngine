#ifndef EDITOR_MODELS_MODEL_H_
#define EDITOR_MODELS_MODEL_H_

#include <map>
#include <string>
#include <vector>
#include <Render/Texture.h>
#include <Render/Mesh.h>

namespace DreamEngine::Editor::Models
{
using namespace DreamEngine::Core::Render;
struct Model
{
    std::vector<Texture*> textures;
    std::vector<Mesh*> meshes;
};
}  // namespace DreamEngine::Editor::Models
#endif