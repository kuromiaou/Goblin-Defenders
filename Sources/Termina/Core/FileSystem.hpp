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

        /// Represents a file watcher that tracks the last write time of a file.
        struct Watch
        {
            std::string Path;
            std::filesystem::file_time_type LastWriteTime;
        };

        /// Returns the current working directory.
        static std::string GetPWD();

        /// Returns `true` if the file has the given extension.
        static bool HasExtension(const std::string& path, const std::string& extension);

        /// Returns `true` if the file exists.
        static bool FileExists(const std::string& path);

        /// Returns `true` if the directory exists.
        static bool DirectoryExists(const std::string& path);

        /// Returns the contents of the file as a vector of bytes.
        static std::vector<uint8> ReadBytesFromDisk(const std::string& path);

        /// Returns the contents of the file as a string.
        static std::string ReadStringFromDisk(const std::string& path);

        /// Writes the given data to the file.
        static void WriteBytesToDisk(const std::string& path, const std::vector<uint8>& data);

        /// Writes the given string to the file.
        static void WriteStringToDisk(const std::string& path, const std::string& data);

        /// Returns the contents of the file as a JSON object.
        static nlohmann::json ReadJSONFromDisk(const std::string& path);

        /// Writes the given JSON object to the file.
        static void WriteJSONToDisk(const std::string& path, const nlohmann::json& json);

        /// Returns a file watcher for the given path.
        static Watch WatchFile(const std::string& path);

        /// Returns `true` if the file has changed since the last watch.
        static bool HasFileChanged(const Watch& watch);

        /// Returns a list of all files in the given directory and its subdirectories.
        static std::vector<std::string> GetFilesRecursive(const std::string& directory);
    };
}
