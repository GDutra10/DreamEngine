#ifndef EDITOR_UI_VIEWS_LOG_VIEW_H_
#define EDITOR_UI_VIEWS_LOG_VIEW_H_

#include <string>
#include <chrono>
#include "../../../Core/Color.h"
#include "../../../Core/Loggers/Log.h"

namespace DreamEngine::Editor::UI::Views
{

using namespace std::chrono;
using namespace DreamEngine::Core;
using namespace DreamEngine::Core::Loggers;

class LogView
{
   public:
    static void Draw(Log& log);
};

}  // namespace DreamEngine::Editor::UI::Views
#endif