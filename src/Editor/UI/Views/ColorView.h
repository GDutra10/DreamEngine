#ifndef EDITOR_UI_VIEWS_COLOR_VIEW_H_
#define EDITOR_UI_VIEWS_COLOR_VIEW_H_

#include <string>
#include "../../../Core/Color.h"

namespace DreamEngine::Editor::UI::Views
{

using namespace DreamEngine::Core;

class ColorView
{
   public:    
    static void Draw(const std::string& text, Color& color);
    static void Draw(const std::string& text, glm::vec3& color);
};

}  // namespace DreamEngine::Editor::UI::Views
#endif