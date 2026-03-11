#pragma once

#include "ShaderPreprocessor.hpp"
#include "ShaderCompiler.hpp"

namespace Termina {
    struct ShaderFile
    {
        struct Entry
        {
            std::unordered_map<ShaderType, std::pair<std::string, std::vector<uint8>>> Bytecodes;
            std::string VariantKey;
        };

        std::string Path;
        std::unordered_map<std::string, Entry> Variants;

        bool Load(const std::string& path);

        Entry GetDefaultEntry();
        Entry GetEntryWithVariants(const std::vector<std::string>& variants);
    };
}
