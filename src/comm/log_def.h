#pragma once

#include "spdlog/spdlog.h"

#if 1


namespace logdef
{   
    inline void init_log()
    {
        spdlog::set_pattern("[%H:%M:%S.%e] [%^%l%$] [%s:%#] %v");
        spdlog::set_level(spdlog::level::trace);
    }
}


#define LOG_TRACE(...)      SPDLOG_TRACE(__VA_ARGS__)
#define LOG_DEBUG(...)      SPDLOG_DEBUG(__VA_ARGS__)
#define LOG_INFO(...)       SPDLOG_INFO(__VA_ARGS__)
#define LOG_WARN(...)       SPDLOG_WARN(__VA_ARGS__)
#define LOG_ERROR(...)      SPDLOG_ERROR(__VA_ARGS__)
#define LOG_CRITICAL(...)   SPDLOG_CRITICAL(__VA_ARGS__)


#else

#define LOG_TRACE(fmt, ...)    spdlog::trace   ("[{}:{}] " fmt, __FILE__, __LINE__, ##__VA_ARGS__)
#define LOG_DEBUG(fmt, ...)    spdlog::debug   ("[{}:{}] " fmt, __FILE__, __LINE__, ##__VA_ARGS__)
#define LOG_INFO(fmt, ...)     spdlog::info    ("[{}:{}] " fmt, __FILE__, __LINE__, ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...)    spdlog::error   ("[{}:{}] " fmt, __FILE__, __LINE__, ##__VA_ARGS__)
#define LOG_CRITICAL(fmt, ...) spdlog::critical("[{}:{}] " fmt, __FILE__, __LINE__, ##__VA_ARGS__)

#endif
