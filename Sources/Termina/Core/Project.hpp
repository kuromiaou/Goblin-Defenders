#pragma once

#include <string>

namespace Termina {
    struct Project
    {
        std::string Name;
        std::string Path;
        std::string LastOpenedScene; // Editor
        std::string FirstSceneToLoad; // Runtime
        std::string AssetPath;

        // TODO: Project settings

        void NewProject(const std::string& name, const std::string& path);
        void LoadProject(const std::string& path);
    };
}
