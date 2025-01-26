#include "FileHelper.h"

using namespace DreamEngine::Editor::Helpers;

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