#include "RenderViewProvider.h"

using namespace DreamEngine::Core::Render;

RenderViewProvider RenderViewProvider::m_instance;

void RenderViewProvider::Initialize()
{
    auto renderView = new RenderView();
    renderView->mask = RenderMask::World | RenderMask::UI;

    m_instance.m_sRenderViews.push_back(renderView);
}

void RenderViewProvider::Add(RenderView* renderView)
{
    m_instance.m_sRenderViews.push_back(renderView);
}

std::vector<RenderView*> RenderViewProvider::GetAll()
{
    return m_instance.m_sRenderViews;
}