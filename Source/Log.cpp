#include "Log.h"

#include <iostream>
#include <mutex>

namespace
{
    std::vector<std::string> messages;
    std::mutex logMutex;
}

void Log(const char file[], int line, const char* format, ...)
{
    char buffer[4096];

    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    std::string logMessage =
        std::string(file) + "(" +
        std::to_string(line) + ") : " +
        buffer;

    {
        std::lock_guard<std::mutex> lock(logMutex);
        messages.push_back(logMessage);
    }

    // Mantener también la salida en terminal.
    std::cerr << logMessage << std::endl;
}

namespace Logger
{
    const std::vector<std::string>& GetMessages()
    {
        return messages;
    }

    void Clear()
    {
        std::lock_guard<std::mutex> lock(logMutex);
        messages.clear();
    }
}