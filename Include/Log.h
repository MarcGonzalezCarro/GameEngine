#ifndef __LOG_H__
#define __LOG_H__

#include <cstdio>
#include <cstdarg>
#include <string>
#include <vector>

#define LOG(format, ...) Log(__FILE__, __LINE__, format, ##__VA_ARGS__)

void Log(const char file[], int line, const char* format, ...);

namespace Logger
{
    const std::vector<std::string>& GetMessages();
    void Clear();
}

#endif