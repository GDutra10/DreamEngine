#ifndef EDITOR_UI_WINDOWS_GAME_WINDOW_H_
#define EDITOR_UI_WINDOWS_GAME_WINDOW_H_

#include "BaseWindow.h"

namespace DreamEngine::Editor::UI::Windows
{
using namespace std;

class GameWindow : public BaseWindow
{
   public:
    GameWindow(const string& title);

   protected:
    void DrawContent() override;
};

}  // namespace DreamEngine::Editor::UI::Windows
#endif