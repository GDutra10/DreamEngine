#include "FileHelper.h"

#include <filesystem>

#include "../Singletons/EditorSingleton.h"

using namespace DreamEngine::Editor::Helpers;
using namespace DreamEngine::Editor::Singletons;

vector<string> FileHelper::GetFilesWithExtension(const path& directory, const string& extension)
{
    vector<string> files;

    for (const auto& entry : recursive_directory_iterator(directory))
    {
        if (entry.is_regular_file() && entry.path().extension() == extension)
        {
            files.push_back(entry.path().string());
        }
    }

    return files;
}

vector<path> FileHelper::GetAllFilesInCurrentDirectory(const path& directoryPath)
{
    vector<path> files;

    // Use a recursive directory iterator to iterate through all files and directories
    for (const auto& entry : directory_iterator(directoryPath))
    {
        if (entry.is_regular_file())  // Check if the entry is a regular file
        {
            files.push_back(entry.path());
        }
    }

    return files;
}

path FileHelper::GetRelativePathByProject(const path& filePath)
{
    return relative(filePath, EditorSingleton::Instance().GetProjectConfiguration().projectPath);
}

#if defined(_WIN32) || defined(_WIN64)

path FileHelper::GetExecutablePath()
{
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    return std::filesystem::path(buffer).parent_path();
}
#elif defined(__linux__)
#include <unistd.h>
#include <limits.h>

path FileHelper::GetExecutablePath()
{
    char buffer[MAX_PATH];
    ssize_t count = readlink("/proc/self/exe", buffer, PATH_MAX);
    return std::filesystem::path(std::string(buffer, (count > 0) ? count : 0)).parent_path();
}
#endif
