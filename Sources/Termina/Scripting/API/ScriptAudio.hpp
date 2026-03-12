#pragma once

#include <Termina/Audio/AudioSystem.hpp>

namespace TerminaScript {
    /// Provides access to audio-related functionality.
    class Audio
    {
    public:
        /// Sets the global audio volume.
        static void SetVolume(float volume);
    };
}
