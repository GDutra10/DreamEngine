#ifndef EDITOR_HELPERS_TEXTURE_HELPER_H_
#define EDITOR_HELPERS_TEXTURE_HELPER_H_

#include <string>
#include "../../Core/Render/Texture.h"

namespace DreamEngine::Editor::Helpers
{
using namespace DreamEngine::Core::Render;
class TextureHelper
{
   public:
    // Load texture from file by current exe directory (current working directory + file path)
    static Texture* LoadTextureFromFile(const std::string& filePath);

   private:
    
};

}  // namespace DreamEngine::Editor::Helpers
#endif