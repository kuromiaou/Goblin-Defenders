#pragma once

#include <string>

namespace TerminaScript {
    /// A reference to a world asset (.trw file). Use as a component field
    /// to allow transitioning to a different scene at runtime.
    struct Scene {
        std::string Path;

        Scene() = default;
        explicit Scene(const std::string& path) : Path(path) {}
        Scene(const char* path) : Path(path) {}

        bool IsValid() const { return !Path.empty(); }
        operator bool() const { return IsValid(); }

        /// Renders a label and a drop-target button for this scene in the inspector.
        /// Returns true if the path was changed by a drag-drop.
        bool Inspect(const char* label);
    };
}
