#pragma once

#include <vector>
#include <string>

namespace Termina {
    /// Represents a process to be launched.
    class LaunchProcess
    {
    public:
        /// Launches a process with the given executable and arguments.
        /// \param executable The path to the executable to launch.
        /// \param arguments The arguments to pass to the executable.
        /// \return The process ID of the launched process, or 0 if an error occurred.
        static int Launch(const std::string& executable, const std::vector<std::string>& arguments);
    };
}
