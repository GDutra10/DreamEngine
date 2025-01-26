#include "TextureHelper.h"

#include <filesystem>

#include "Application.h"
#include "../Vendors/stb_image.h"

using namespace DreamEngine::Editor::Helpers;

Texture* TextureHelper::LoadTextureFromFile(const std::string& filePath)
{
    stbi_set_flip_vertically_on_load(true);

    const std::string path = std::filesystem::current_path().string() + "\\" + filePath;
    int width, height, channels;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    Texture* texture = Core::Application::Instance().GetRenderAPI()->CreateTexture(data, width, height, channels);
    stbi_image_free(data);

    return texture;
}