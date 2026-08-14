#include "TextureImporter.h"

#include "Application.h"
#include "../Vendors/stb_image.h"

using namespace DreamEngine;
using namespace DreamEngine::Core;
using namespace DreamEngine::Core::Render;
using namespace DreamEngine::Editor;
using namespace DreamEngine::Editor::Importers;

Texture* TextureImporter::Import(std::string const& path, const char* fileName, TextureType& textureType)
{
    Texture* texture = nullptr;
    int width, height, nrComponents;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);

    if (data != nullptr)
    {
        texture = Application::Instance().GetRenderAPI()->CreateTexture(data, width, height, nrComponents);
        texture->type = textureType;
        texture->name = fileName;
        texture->path = path;

        stbi_image_free(data);
    }

    return texture;
}
