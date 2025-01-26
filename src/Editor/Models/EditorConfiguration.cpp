#include "EditorConfiguration.h"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace DreamEngine::Editor::Models;

const std::string EDITOR_CONFIG_FILE = "editor.ini";

EditorConfiguration EditorConfiguration::LoadEditorConfiguration()
{
    EditorConfiguration config;

    std::ifstream stream(EDITOR_CONFIG_FILE);

    //  .ini file https://en.wikipedia.org/wiki/INI_file
    if (stream.is_open())
    {
        std::string temp;

        while (std::getline(stream, temp))
        {
            // Ignore comments and sections
            if (temp.empty() || temp[0] == ';' || temp[0] == '[')
                continue;

            if (temp.find("WindowSize") != std::string::npos)
            {
                std::string sizeStr = temp.substr(temp.find('=') + 1);
                std::istringstream sizeStream(sizeStr);
                std::string widthStr, heightStr;

                if (std::getline(sizeStream, widthStr, ',') &&
                    std::getline(sizeStream, heightStr))
                {
                    config.windowSize.x = std::stof(widthStr);
                    config.windowSize.y = std::stof(heightStr);
                }
            }
        }
    }
    else
    {
        // Set default values
        config.windowSize = {1200.0f, 800.0f};

        // Create and write default values to the new file
        std::ofstream outFile(EDITOR_CONFIG_FILE);
        if (outFile.is_open())
        {
            outFile << "WindowSize=" << config.windowSize.x << "," << config.windowSize.y << '\n';
            outFile.close();
        }
        else
            throw std::exception("Failed to create 'init.editor' file.");
    }

    return config;
}