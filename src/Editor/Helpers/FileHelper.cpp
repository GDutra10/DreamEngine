#include "FileHelper.h"

#include <filesystem>
#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <string>
#include <algorithm>
#include "../Models/ProjectConfiguration.h"
#include "../../Core/Loggers/LoggerSingleton.h"

using namespace DreamEngine::Core::Loggers;
using namespace DreamEngine::Editor::Models;
using namespace DreamEngine::Editor::Helpers;

vector<string> FileHelper::GetFilesWithExtension(const path& directory, const string& extension)
{
    std::vector<string> extensions{extension};

    return GetFilesWithExtensions(directory, extensions);
}

vector<string> FileHelper::GetFilesWithExtensions(const path& directory, const std::vector<string>& extension)
{
    vector<string> files;

    for (const auto& entry : recursive_directory_iterator(directory))
    {
        if (entry.is_regular_file() && IsExpectedExtension(entry.path(), extension))
            files.push_back(entry.path().string());
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

path FileHelper::GetRelativePathByProject(const path& filePath, const ProjectConfiguration& projectConfig)
{
    return relative(filePath, projectConfig.projectPath);
}

std::vector<unsigned char> FileHelper::LoadFileIntoVector(const std::string& filename)
{
    std::ifstream inputFile(filename, std::ios_base::binary);
    auto fileSize = std::filesystem::file_size(filename);
    std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(inputFile), {});

    return buffer;
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


Result FileHelper::CreateFile(const std::string& filePath, const std::string& filename, const std::string& content)
{
    string pathAndFileName = filePath + "\\" + filename;

    // validations
    if (filename.empty())
    {
        LoggerSingleton::Instance().LogWarning("Filename is empty");
        return {"File name is empty", false};
    }

    if (exists(pathAndFileName))
    {
        std::string validation = "File already exists in this directory";
        LoggerSingleton::Instance().LogWarning(validation);
        return {validation, false};
    }

    Result result = {"", true};
    std::ofstream file(pathAndFileName);

    if (file.is_open())
    {
        file << content;
        file.close();

        LoggerSingleton::Instance().LogTrace("FileHelper::CreateFile -> File '" + pathAndFileName + "' saved");
    }
    else
    {
        LoggerSingleton::Instance().LogError("Failed to open the file");
        result.errorMessage = "Failed to open the file";
        result.isOk = false;
    }

    return result;
}

bool FileHelper::IsExpectedExtension(const path& path, const std::vector<std::string>& extensionsExpected)
{
    if (std::find(extensionsExpected.begin(), extensionsExpected.end(), path.extension()) != extensionsExpected.end())
        return true;
    
    return false;
}
