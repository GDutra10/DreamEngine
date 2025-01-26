#ifndef EDITOR_HELPERS_FILE_HELPER_H_
#define EDITOR_HELPERS_FILE_HELPER_H_

#include <string>
#include <vector>
#include <filesystem>

namespace DreamEngine::Editor::Helpers
{
    using namespace std;
    using namespace std::filesystem;
class FileHelper
{
   public:
    static vector<string> GetFilesWithExtension(const path& directory, const string& extension);
    static vector<path> GetAllFilesInCurrentDirectory(const path& directoryPath);

   private:
   
};

}  // namespace DreamEngine::Editor::Helpers
#endif