#ifndef CORE_RENDER_HELPERS_RENDER_TYPE_HELPER_H_
#define CORE_RENDER_HELPERS_RENDER_TYPE_HELPER_H_

#include "../Mesh.h"
#include "../RenderType.h"

namespace DreamEngine::Core::Render::Helpers
{
using namespace DreamEngine::Core::Render;

class CORE_API RenderTypeHelper
{
   public:
    static std::string ToString(const RenderType renderType)
    {
        switch (renderType)
        {
            case openGL:
                return "OpenGL";
            case vulkan:
                return "Vulkan";
            case none:
                return "None";
            default:
                return "UNKOWN";
        }
    }
};
}  // namespace DreamEngine::Core::Render::Factories
#endif