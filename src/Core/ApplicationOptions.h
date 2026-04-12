#ifndef CORE_APPLICATION_OPTION_H_
#define CORE_APPLICATION_OPTION_H_

#include "CoreExport.h"
#include <string>
#include "Render/RenderView.h"

namespace DreamEngine::Core
{

using namespace DreamEngine::Core::Render;

class CORE_API ApplicationOptions
{
   public:
    uint32_t DefaultRenderMask = RenderMask::World | RenderMask::UI;
};
}  // namespace DreamEngine::Core
#endif