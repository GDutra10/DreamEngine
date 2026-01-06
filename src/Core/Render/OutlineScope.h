#ifndef CORE_RENDER_OUTLINE_SCOPE_H_
#define CORE_RENDER_OUTLINE_SCOPE_H_

#include "CoreExport.h"
#include "RenderAPI.h"

namespace DreamEngine::Core::Render
{
// RAII class to handle outline rendering pass
class CORE_API OutlineScope
{
   public:
    OutlineScope(RenderAPI& api, const OutlineOptions& opts) : m_renderApi(api) { m_renderApi.OutlineBeginPass(opts); }
    ~OutlineScope() { m_renderApi.OutlineEndPass(); }

   private:
    RenderAPI& m_renderApi;
};
}  // namespace DreamEngine::Core::Render
#endif