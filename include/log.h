#pragma once

#ifdef _MSC_VER 
    #define FMT_UNICODE 0
#endif

#include <spdlog/spdlog.h>



#define DEFAULT_LOGGER_NAME "SpaceEngineLogger"

#ifdef SPACE_ENGINE_DEBUG
#define SPACE_ENGINE_TRACE(...)\
            if(spdlog::get(DEFAULT_LOGGER_NAME) != nullptr)\
            {\
                spdlog::get(DEFAULT_LOGGER_NAME)->trace(__VA_ARGS__);\
            }
#else
#define SPACE_ENGINE_TRACE(...) (void)0
#endif