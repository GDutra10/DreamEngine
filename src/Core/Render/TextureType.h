#ifndef CORE_RENDER_TEXTURE_TYPE_H_
#define CORE_RENDER_TEXTURE_TYPE_H_

#include "CoreExport.h"

namespace DreamEngine::Core::Render
{

enum CORE_API TextureType
{
   Diffuse,
   Specular,
   Normal,
   Height
};
}  // namespace DreamEngine::Core::Render
#endif