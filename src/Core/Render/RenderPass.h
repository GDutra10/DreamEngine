#ifndef CORE_RENDER_RENDER_PASS_H_
#define CORE_RENDER_RENDER_PASS_H_

#include "CoreExport.h"
#include "RenderAPI.h"
#include "RenderView.h"

namespace DreamEngine::Core::Render
{
class CORE_API RenderPass
{
   public:
    void RegisterDebugPass(std::function<void(Scene&, RenderView& renderView, RenderAPI* renderApi)> fnDebugPass) { m_fnDebugPass = fnDebugPass;}

   protected:
    std::function<void(Scene&, RenderView& renderView, RenderAPI* renderApi)> m_fnDebugPass;
};
}  // namespace DreamEngine::Core::Render
#endif