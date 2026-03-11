#include "Assert.hpp"
#include "Logger.hpp"

#include <cstdarg>
#include <cstdio>
#include <cstdlib>

namespace Termina {

void Assert::ReportFailure(const char* condition, const char* file, int line, const char* format, ...) {
    char messageBuffer[4096] = {0};
    
    if (format) {
        va_list args;
        va_start(args, format);
        std::vsnprintf(messageBuffer, sizeof(messageBuffer), format, args);
        va_end(args);
    }

    if (messageBuffer[0] != '\0') {
        Logger::Fatal(file, line, "Assertion Failed: %s. Message: %s", condition, messageBuffer);
    } else {
        Logger::Fatal(file, line, "Assertion Failed: %s", condition);
    }
    
    Logger::Flush();

#if defined(_MSC_VER)
    __debugbreak();
#elif defined(__APPLE__) || defined(__linux__) || defined(__GNUC__) || defined(__clang__)
    __builtin_trap();
#else
    std::abort();
#endif
}

} // namespace Termina