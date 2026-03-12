#pragma once

#include "Common.hpp"

#include <string>

namespace Termina {

    /// Logging level for the logger.
    enum class ELogLevel : uint8_t
    {
        kDebug,
        kTrace,
        kInfo,
        kWarning,
        kError,
        kFatal,
    };

    class Logger
    {
    public:
        /// Initializes the logger with the given log path and log level.
        static void Initialize(const std::string& logPath, ELogLevel logLevel);

        /// Flushes the log file and console buffers.
        static void Flush();

        static void Info(const char* file, int line, const char* format, ...);
        static void Debug(const char* file, int line, const char* format, ...);
        static void Trace(const char* file, int line, const char* format, ...);
        static void Warning(const char* file, int line, const char* format, ...);
        static void Error(const char* file, int line, const char* format, ...);
        static void Fatal(const char* file, int line, const char* format, ...);
    private:
        static void Output(ELogLevel level, const char* file, int line, const char* message);
    };
}

// Logging Macros to automatically include file and line number
#define TN_DEBUG(...) ::Termina::Logger::Debug(__FILE__, __LINE__, __VA_ARGS__)
#define TN_TRACE(...) ::Termina::Logger::Trace(__FILE__, __LINE__, __VA_ARGS__)
#define TN_INFO(...)  ::Termina::Logger::Info(__FILE__, __LINE__, __VA_ARGS__)
#define TN_WARN(...)  ::Termina::Logger::Warning(__FILE__, __LINE__, __VA_ARGS__)
#define TN_ERROR(...) ::Termina::Logger::Error(__FILE__, __LINE__, __VA_ARGS__)
#define TN_FATAL(...) ::Termina::Logger::Fatal(__FILE__, __LINE__, __VA_ARGS__)
