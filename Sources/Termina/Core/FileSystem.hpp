#pragma once

#include <filesystem>
#include <string>
#include <vector>

#include <JSON/json.hpp>

#include "Common.hpp"

namespace Termina {
    class FileSystem
    {
    public:
        // TODO: Load from archive with manifest, ReadBytes generic function that picks between disk/archive
        
        struct Watch
        {
            std::string Path;
            std::filesystem::file_time_type LastWriteTime;
        };

        static std::string GetPWD();
        static bool HasExtension(const std::string& path, const std::string& extension);

        static bool FileExists(const std::string& path);
        static bool DirectoryExists(const std::string& path);

        static std::vector<uint8> ReadBytesFromDisk(const std::string& path);
        static std::string ReadStringFromDisk(const std::string& path);

        static void WriteBytesToDisk(const std::string& path, const std::vector<uint8>& data);
        static void WriteStringToDisk(const std::string& path, const std::string& data);

        static nlohmann::json ReadJSONFromDisk(const std::string& path);
        static void WriteJSONToDisk(const std::string& path, const nlohmann::json& json);

        static Watch WatchFile(const std::string& path);
        static bool HasFileChanged(const Watch& watch);

        static std::vector<std::string> GetFilesRecursive(const std::string& directory);
    };
}
