#include "FileSystem.hpp"
#include "Logger.hpp"

#include <fstream>

namespace Termina {
    std::string FileSystem::GetPWD()
    {
        return std::filesystem::current_path().string();
    }

    bool FileSystem::HasExtension(const std::string& path, const std::string& extension)
    {
        return std::filesystem::path(path).extension() == extension;
    }

    bool FileSystem::FileExists(const std::string& path)
    {
        return std::filesystem::exists(path) && std::filesystem::is_regular_file(path);
    }

    bool FileSystem::DirectoryExists(const std::string& path)
    {
        return std::filesystem::exists(path) && std::filesystem::is_directory(path);
    }

    std::vector<uint8> FileSystem::ReadBytesFromDisk(const std::string& path)
    {
        std::ifstream file(path, std::ios::binary | std::ios::ate);
        if (!file) {
            TN_ERROR("Failed to open file: %s", path.c_str());
            return {};
        }

        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);
        std::vector<uint8> buffer(size);
        if (file.read(reinterpret_cast<char*>(buffer.data()), size)) {
            return buffer;
        } else {
            TN_ERROR("Failed to read file: %s", path.c_str());
            return {};
        }
    }

    std::string FileSystem::ReadStringFromDisk(const std::string& path)
    {
        std::ifstream file(path, std::ios::binary | std::ios::ate);
        if (!file) {
            TN_ERROR("Failed to open file: %s", path.c_str());
            return {};
        }

        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);
        std::string buffer(size, '\0');
        if (file.read(&buffer[0], size)) {
            return buffer;
        } else {
            TN_ERROR("Failed to read file: %s", path.c_str());
            return {};
        }
    }

    void FileSystem::WriteBytesToDisk(const std::string& path, const std::vector<uint8>& data)
    {
        std::ofstream file(path, std::ios::binary | std::ios::trunc);
        if (!file) {
            TN_ERROR("Failed to open file for writing: %s", path.c_str());
            return;
        }
        file.write(reinterpret_cast<const char*>(data.data()), data.size());
        if (!file) {
            TN_ERROR("Failed to write data to file: %s", path.c_str());
        }
    }

    void FileSystem::WriteStringToDisk(const std::string& path, const std::string& data)
    {
        std::ofstream file(path, std::ios::binary | std::ios::trunc);
        if (!file) {
            TN_ERROR("Failed to open file for writing: %s", path.c_str());
            return;
        }
        file.write(data.data(), data.size());
        if (!file) {
            TN_ERROR("Failed to write data to file: %s", path.c_str());
        }
    }

    nlohmann::json FileSystem::ReadJSONFromDisk(const std::string& path)
    {
        std::string jsonString = ReadStringFromDisk(path);
        if (jsonString.empty()) {
            TN_ERROR("Failed to read JSON file: %s", path.c_str());
            return {};
        }
        try {
            return nlohmann::json::parse(jsonString);
        } catch (const nlohmann::json::parse_error& e) {
            TN_ERROR("Failed to parse JSON file: %s, error: %s", path.c_str(), e.what());
            return {};
        }
    }

    void FileSystem::WriteJSONToDisk(const std::string& path, const nlohmann::json& json)
    {
        std::string jsonString = json.dump(4);
        WriteStringToDisk(path, jsonString);
    }

    FileSystem::Watch FileSystem::WatchFile(const std::string& path)
    {
        Watch watch;
        watch.Path = path;
        watch.LastWriteTime = std::filesystem::last_write_time(path);
        return watch;
    }

    bool FileSystem::HasFileChanged(const Watch& watch)
    {
        auto currentLastWriteTime = std::filesystem::last_write_time(watch.Path);
        return currentLastWriteTime != watch.LastWriteTime;
    }

    std::vector<std::string > FileSystem::GetFilesRecursive(const std::string& directory)
    {
        std::vector<std::string> files;
        for (const auto& entry : std::filesystem::recursive_directory_iterator(directory)) {
            if (entry.is_regular_file()) {
                files.push_back(entry.path().string());
            }
        }
        return files;
    }
}
