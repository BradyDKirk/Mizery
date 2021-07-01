#include "log.h"

namespace Mizery {
    
    // Static vars
    std::shared_ptr<spdlog::logger> Log::engineLogger;
    
    void Log::init()
    {
        spdlog::set_pattern("%^[%T] %n: %v%$");
        
        engineLogger = spdlog::stdout_color_mt("Engine");
        
#ifdef MZ_BUILD_REL
        engineLogger->set_level(spdlog::level::info);
#else
        engineLogger->set_level(spdlog::level::trace);
#endif
    }
    
}