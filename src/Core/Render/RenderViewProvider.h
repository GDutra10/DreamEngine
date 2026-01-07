#ifndef CORE_RENDER_RENDER_VIEW_PROVIDER_H_
#define CORE_RENDER_RENDER_VIEW_PROVIDER_H_

#include <vector>

#include "CoreExport.h"
#include "RenderView.h"

namespace DreamEngine::Core::Render
{

class CORE_API RenderViewProvider
{
public:
    static void Initialize();
    static void Add(RenderView* renderView);
    static std::vector<RenderView*> GetAll();
private:
    std::vector<RenderView*> m_sRenderViews;
    static RenderViewProvider m_instance;
};
}  // namespace DreamEngine::Core::Render
#endif