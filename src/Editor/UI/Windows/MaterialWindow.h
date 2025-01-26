#ifndef EDITOR_UI_WINDOWS_MATERIAL_WINDOW_H_
#define EDITOR_UI_WINDOWS_MATERIAL_WINDOW_H_

#include "BaseWindow.h"

#include <filesystem>

#include "../../Core/Render/Material.h"

namespace DreamEngine::Editor::UI::Windows
{
using namespace std;
using namespace std::filesystem;
using namespace DreamEngine::Core::Render;

class MaterialWindow : public BaseWindow
{
   public:
    MaterialWindow(const std::string& title);

   protected:
    void DrawContent() override;

   private:
    Material* m_pMaterial;
    Material* m_originalMaterial;
    path m_sSelectedMaterialPath;
    void LoadMaterialFromFile();
};

}  // namespace DreamEngine::Editor::UI::Windows
#endif