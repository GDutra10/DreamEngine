#ifndef EDITOR_IMPORTERS_TEXTURE_IMPORTER_H_
#define EDITOR_IMPORTERS_TEXTURE_IMPORTER_H_

#include <string>
#include "../../Core/Render/Texture.h"
#include "../../Core/Render/TextureType.h"

namespace DreamEngine::Editor::Importers
{
    using namespace DreamEngine::Core::Render;
class TextureImporter
{
   public:
    static Texture* Import(std::string const& path, const char* fileName, TextureType& textureType);
};

}  // namespace DreamEngine::Editor::Importers
#endif