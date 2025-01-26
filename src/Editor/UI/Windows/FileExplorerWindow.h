#ifndef EDITOR_UI_WINDOWS_FILE_EXPLORER_WINDOW_H_
#define EDITOR_UI_WINDOWS_FILE_EXPLORER_WINDOW_H_

#include <filesystem>
#include <functional>
#include <vector>
#include <string>
#include <map>

#include "BaseWindow.h"
#include "../../Models/FileIcon.h"

namespace DreamEngine::Editor::UI::Windows
{
using namespace std;
using namespace std::filesystem;
using namespace DreamEngine::Editor::Models;
class FileExplorerWindow final : public BaseWindow
{
   public:
    FileExplorerWindow(const std::string& title);

   protected:
    void DrawContent() override;

   private:
    path m_sSelectedFile;
    path m_sRightClickFile;
    bool m_wasInitialized = false;
    std::map<std::string, FileIcon> m_mFileIcons;
    void DrawBreadcrumbNavigation(const path& currentPath);
    void FileSystemBrowser();
    void DrawFileEntry(const std::string& name, const FileIcon& icon, bool isSelected, const function<void()>& onClick);
};

}  // namespace DreamEngine::Editor::UI::Windows
#endif