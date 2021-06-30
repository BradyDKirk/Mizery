#pragma once

#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#pragma warning(pop)

#include <memory>

namespace Mizery {
    
    class Log
    {
    public:
        static void init();

        static std::shared_ptr<spdlog::logger>& getEngineLogger() { return engineLogger; }
    private:
        static std::shared_ptr<spdlog::logger> engineLogger;
    };
    
}

#ifndef MZ_BUILD_DST
    #define LOG_TRACE(...)    Log::getEngineLogger()->trace(__VA_ARGS__)
    #define LOG_DEBUG(...)    Log::getEngineLogger()->debug(__VA_ARGS__)
    #define LOG_INFO(...)     Log::getEngineLogger()->info(__VA_ARGS__)
    #define LOG_WARN(...)     Log::getEngineLogger()->warn(__VA_ARGS__)
    #define LOG_ERROR(...)    Log::getEngineLogger()->error(__VA_ARGS__)
    #define LOG_CRITICAL(...) Log::getEngineLogger()->critical(__VA_ARGS__)
#else
    #define LOG_TRACE
    #define LOG_DEBUG
    #define LOG_INFO
    #define LOG_WARN
    #define LOG_ERROR
    #define LOG_CRITICAL
#endif
