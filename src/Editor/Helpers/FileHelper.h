#ifndef EDITOR_HELPERS_FILE_HELPER_H_
#define EDITOR_HELPERS_FILE_HELPER_H_

#include <string>
#include <vector>
#include <filesystem>
#include "../Models/ProjectConfiguration.h"

#define MAX_PATH 260

typedef void* HMODULE;
typedef void* FARPROC;
typedef unsigned long DWORD;

extern "C"
{
    __declspec(dllimport) DWORD __stdcall GetModuleFileNameA(HMODULE hModule, char* lpFilename, DWORD nSize);
}

namespace DreamEngine::Editor::Helpers
{
    using namespace std;
    using namespace std::filesystem;
    using namespace DreamEngine::Editor::Models;
class FileHelper
{
   public:
    static vector<string> GetFilesWithExtension(const path& directory, const string& extension);
    static vector<path> GetAllFilesInCurrentDirectory(const path& directoryPath);
    static path GetExecutablePath();
    static path GetRelativePathByProject(const path& filePath, const ProjectConfiguration& projectConfig);
    static std::vector<unsigned char> LoadFileIntoVector(const std::string& filename);

   private:
   
};

}  // namespace DreamEngine::Editor::Helpers
#endif