#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <string>

namespace Zephyrus
{
    class Log
    {
    public:
        enum class LogType {
            ZLT_INFO,
            ZLT_LOAD,
            ZLT_WARN,
            ZLT_ERROR,
            ZLT_ASSERT
        };

        enum class Logger {
            ZP_ZEPHYRUS,
            ZP_APP,
            ZP_GAME
        };
        static void Init();
        static void Shutdown();

        static void Info(Logger pLogger, const std::string& pMessage);

        static void Load(Logger pLogger, const std::string& pMessage);

        static void Warn(Logger pLogger, const std::string& pMessage, const char* pFile, int pLine);

        static void Error(Logger pLogger, const std::string& pMessage, const char* pFile, int pLine);

        static void Assert(Logger pLogger, bool pCondition, const std::string& pMessage, const char* pFile, int pLine);
    private:

        static std::string GetTimestamp();

        static std::string GetColorCode(LogType pType);

        static std::string LevelToString(LogType pType);

        static void WriteLog(Logger pLogger, LogType pType, const std::string& pMessage, const char* pFile, int pLine);
    };
}
#define ZP_CORE_INFO(msg)               Zephyrus::Log::Info(Zephyrus::Log::Logger::ZP_ZEPHYRUS, msg)
#define ZP_CORE_LOAD(msg)               Zephyrus::Log::Load(Zephyrus::Log::Logger::ZP_ZEPHYRUS, msg)
#define ZP_CORE_WARN(msg)               Zephyrus::Log::Warn(Zephyrus::Log::Logger::ZP_ZEPHYRUS, msg, __FILE__, __LINE__)
#define ZP_CORE_ERROR(msg)              Zephyrus::Log::Error(Zephyrus::Log::Logger::ZP_ZEPHYRUS, msg, __FILE__, __LINE__)
#define ZP_CORE_ASSERT(cond, msg)       Zephyrus::Log::Assert(Zephyrus::Log::Logger::ZP_ZEPHYRUS, cond, msg, __FILE__, __LINE__)

#define ZP_EDITOR_INFO(msg)             Zephyrus::Log::Info(Zephyrus::Log::Logger::ZP_APP, msg)
#define ZP_EDITOR_LOAD(msg)             Zephyrus::Log::Load(Zephyrus::Log::Logger::ZP_APP, msg)
#define ZP_EDITOR_WARN(msg)             Zephyrus::Log::Warn(Zephyrus::Log::Logger::ZP_APP, msg, __FILE__, __LINE__)
#define ZP_EDITOR_ERROR(msg)            Zephyrus::Log::Error(Zephyrus::Log::Logger::ZP_APP, msg, __FILE__, __LINE__)
#define ZP_EDITOR_ASSERT(cond, msg)     Zephyrus::Log::Assert(Zephyrus::Log::Logger::ZP_APP, cond, msg, __FILE__, __LINE__)

#define ZP_INFO(msg)                    Zephyrus::Log::Info(Zephyrus::Log::Logger::ZP_GAME, msg)
#define ZP_LOAD(msg)                    Zephyrus::Log::Load(Zephyrus::Log::Logger::ZP_GAME, msg)
#define ZP_WARN(msg)                    Zephyrus::Log::Warn(Zephyrus::Log::Logger::ZP_GAME, msg, __FILE__, __LINE__)
#define ZP_ERROR(msg)                   Zephyrus::Log::Error(Zephyrus::Log::Logger::ZP_GAME, msg, __FILE__, __LINE__)
#define ZP_ASSERT(cond, msg)            Zephyrus::Log::Assert(Zephyrus::Log::Logger::ZP_GAME, cond, msg, __FILE__, __LINE__)
