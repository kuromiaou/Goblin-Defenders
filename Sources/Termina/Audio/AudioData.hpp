#pragma once

#include <Termina/Core/Common.hpp>
#include <MiniAudio/miniaudio.h>

#include <string>

namespace Termina {
    /// Represents an audio file loaded into memory for playback.
    class AudioData
    {
    public:
        /// Constructs an AudioData object from a file path.
        AudioData(ma_engine& engine, const std::string& path);
        ~AudioData();

        bool IsValid() const { return m_IsValid; }

        ma_decoder& GetDecoder() { return m_Decoder; }
        const ma_decoder& GetDecoder() const { return m_Decoder; }
    private:
        bool m_IsValid = false;
        ma_decoder m_Decoder;
    };
}
