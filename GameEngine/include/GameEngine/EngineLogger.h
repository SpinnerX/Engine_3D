#pragma once
#include <GameEngine/core.h>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <GameEngine/Events/Event.h>
// #include <spdlog/sinks/ostream_sink.h>
// #include <spdlog/sinks/rotating_file_sink.h>
// #include <spdlog/sinks/stdout_color_sinks.h>
// #include <spdlog/pattern_formatter.h>
// using spdlog::memory_buf_t;

namespace RendererEngine{

    class RENDER_API EngineLogger{
    public:
        EngineLogger();
        ~EngineLogger();

        static void Init();

        // If either coreLogger or clientLogger is nullptr
        // Then we should automatically terminate the app.
        // This is because if we continue, we will segfault due to the coreLogger/clientLogger not being initialized
        static bool isLoggerInit() {
            return (coreLogger == nullptr || clientLogger == nullptr);
        }

        inline static std::shared_ptr<spdlog::logger>& GetCoreLogger(){
            if(EngineLogger::isLoggerInit()){
                // spdlog::fatal("EngineLogger::Init() needs to be called!\nEngineLogger::Init() returns a nullptr");
                spdlog::log(spdlog::level::critical, "EngineLogger::Init() needs to be called!\n");
                std::terminate();
            }
            return coreLogger;
        }

        inline static std::shared_ptr<spdlog::logger>& GetClientLogger() {
            if(EngineLogger::isLoggerInit()){
                // Logger::fatal("EngineLogger::Init() needs to be called!\n");
                spdlog::log(spdlog::level::critical, "EngineLogger::Init() needs to be called!\n");
                std::terminate();
            }
            
            return clientLogger;
        }

    private:
        // static std::shared_ptr<Logger::Log> coreLogger;
        // static std::shared_ptr<Logger::Log> clientLogger;
        static std::shared_ptr<spdlog::logger> coreLogger;
        static std::shared_ptr<spdlog::logger> clientLogger;
    };
};

// ------------ core logs ------------

// #define CORE_TRACE(...) RendererEngine::EngineLogger::GetCoreLogger()->trace(__VA_ARGS__);


template<typename... T>
inline void coreLogTrace(spdlog::format_string_t<T...> fmt, T&&... args) {
    RendererEngine::EngineLogger::GetCoreLogger()->trace(fmt, std::forward<T>(args)...);
}

template<typename... T>
inline void coreLogInfo(spdlog::format_string_t<T...> fmt, T&&... args) {
    RendererEngine::EngineLogger::GetCoreLogger()->info(fmt, std::forward<T>(args)...);
}

template<typename... T>
inline void coreLogWarn(spdlog::format_string_t<T...> fmt, T &&...args) {
    RendererEngine::EngineLogger::GetCoreLogger()->warn(fmt, std::forward<T>(args)...);
}

template<typename... T>
inline void coreLogError(spdlog::format_string_t<T...> fmt, T &&...args) {
    RendererEngine::EngineLogger::GetCoreLogger()->error(fmt, std::forward<T>(args)...);
}

template<typename... T>
inline void coreLogFatal(spdlog::format_string_t<T...> fmt, T &&...args) {
    // RendererEngine::EngineLogger::GetCoreLogger()->fatal(fmt, std::forward<T>(args)...);
    RendererEngine::EngineLogger::GetCoreLogger()->critical(fmt, std::forward<T>(args)...);
}

// ------------ Client logs ------------
template<typename... T>
inline void clientLogTrace(spdlog::format_string_t<T...> fmt, T &&...args) {
    RendererEngine::EngineLogger::GetClientLogger()->trace(fmt, std::forward<T>(args)...);
}

template<typename... T>
inline void clientLogInfo(spdlog::format_string_t<T...> fmt, T &&...args) {
    RendererEngine::EngineLogger::GetClientLogger()->info(fmt, std::forward<T>(args)...);
}

template<typename... T>
inline void clientLogWarn(spdlog::format_string_t<T...> fmt, T &&...args) {
    RendererEngine::EngineLogger::GetClientLogger()->warn(fmt, std::forward<T>(args)...);
}

template<typename... T>
inline void clientLogError(spdlog::format_string_t<T...> fmt, T &&...args) {
    RendererEngine::EngineLogger::GetClientLogger()->error(fmt, std::forward<T>(args)...);
}

template<typename... T>
inline void clientLogFatal(spdlog::format_string_t<T...> fmt, T &&...args) {
    RendererEngine::EngineLogger::GetClientLogger()->critical(fmt, std::forward<T>(args)...);
}

// log to str and return it
// template<typename... Args>
// std::string toStringLog(const std::string msg, spdlog::level::level_enum lvl, const Args &... args){
//     std::ostringstream oss;
//     auto oss_sink = std::make_shared<spdlog::sinks::ostream_sink_mt>(oss);
//     spdlog::logger oss_logger("pattern_tester", oss_sink);
//     oss_logger.set_level(lvl);

//     oss_logger.set_formatter(std::unique_ptr<spdlog::formatter>(new spdlog::pattern_formatter(args...)));

//     oss_logger.info(msg);
//     return oss.str();
// }
