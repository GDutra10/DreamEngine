#ifndef EDITOR_UI_WINDOWS_MATERIAL_WINDOW_H_
#define EDITOR_UI_WINDOWS_MATERIAL_WINDOW_H_

#include "BaseWindow.h"

#include <filesystem>

#include "../../Core/Render/Material.h"
#include "../../Controllers/ResourceController.h"

namespace DreamEngine::Editor::UI::Windows
{
using namespace std;
using namespace std::filesystem;
using namespace DreamEngine::Core::Render;
using namespace DreamEngine::Editor::Controllers;

class MaterialWindow : public BaseWindow
{
   public:
    MaterialWindow(const std::string& title, EditorContext& editorContext, ResourceController& resourceController);

   protected:
    void DrawContent() override;

   private:
    ResourceController& m_resourceController;
    Material* m_pMaterial;
    Material* m_originalMaterial;
    path m_sSelectedMaterialPath;
    void LoadMaterialFromFile();
};

}  // namespace DreamEngine::Editor::UI::Windows
#endif