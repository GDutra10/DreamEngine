#include "../EditorContext.h"
#include "ResourceController.h"
#ifndef EDITOR_CONTROLLERS_SCRIPT_CONTROLLER_H_
#define EDITOR_CONTROLLERS_SCRIPT_CONTROLLER_H_

namespace DreamEngine::Editor::Controllers
{
class ScriptController
{
   public:
    ScriptController(EditorContext& editorContext, ResourceController& resourceController);
    void ReloadScripts();
   private:
    EditorContext& m_editorContext;
    ResourceController& m_resourceController;
    bool BuildSolution();
};

}  // namespace DreamEngine::Editor::Controllers
#endif