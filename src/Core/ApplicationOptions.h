#ifndef CORE_APPLICATION_OPTION_H_
#define CORE_APPLICATION_OPTION_H_

#include "CoreExport.h"
#include <string>
#include "Render/RenderView.h"
#include "Render/RenderType.h"
#include <WindowConfiguration.h>

namespace DreamEngine::Core
{

using namespace DreamEngine::Core::Render;

class CORE_API ApplicationOptions
{
   public:
    uint32_t defaultRenderMask = RenderMask::World | RenderMask::UI;
    RenderType renderType = RenderType::openGL;
    WindowConfiguration windowConfiguration;
};
}  // namespace DreamEngine::Core
#endif