#ifndef EDITOR_UI_WINDOWS_RESOURCE_MANAGER_WINDOW_H_
#define EDITOR_UI_WINDOWS_RESOURCE_MANAGER_WINDOW_H_

#include <string>
#include <vector>

#include "BaseWindow.h"

#include "../../../Core/Resources/Resource.h"

namespace DreamEngine::Editor::UI::Windows
{
using namespace std;
using namespace DreamEngine::Core::Resources;
class ResourceManagerWindow : public BaseWindow
{
   public:
    ResourceManagerWindow(const string& title);

   protected:
    void DrawContent() override;

   private:
    string m_resourceSelected;
    static vector<string> m_sResourcesNames;
    void DrawMaterials();
    void DrawMeshes();
    void DrawShaders();
    void DrawTextures();
    void DrawScripts();
    
};

}  // namespace DreamEngine::Editor::UI::Windows
#endif