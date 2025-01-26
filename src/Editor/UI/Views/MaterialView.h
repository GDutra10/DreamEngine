#ifndef EDITOR_UI_VIEWS_MATERIAL_VIEW_H_
#define EDITOR_UI_VIEWS_MATERIAL_VIEW_H_

#include <functional>
#include "../../Core/Render/Material.h"

namespace DreamEngine::Editor::UI::Views
{
using namespace std;
using namespace DreamEngine::Core::Render;
class MaterialView
{
   public:
    void Draw(Material& material, const function<void(Material*)>& callbackChange);
};

}  // namespace DreamEngine::Editor::UI::Views
#endif