#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <string>

#include <vector>
#include "ILogListener.h"

#include "LogEnum.h"

namespace Zephyrus::Debug
{
    class Log
    {
    public:
        static void Init();
        static void Shutdown();

        static void Info(Logger pLogger, const std::string& pMessage);

        static void Load(Logger pLogger, const std::string& pMessage);

        static void Warn(Logger pLogger, const std::string& pMessage, const char* pFile, int pLine);

        static void Error(Logger pLogger, const std::string& pMessage, const char* pFile, int pLine);

        static void Assert(Logger pLogger, bool pCondition, const std::string& pMessage, const char* pFile, int pLine);

        static void AddListener(ILogListener* listener);
    private:

        static std::string GetTimestamp();

        static std::string GetColorCode(LogType pType);

        static std::string LevelToString(LogType pType);

        static void WriteLog(Logger pLogger, LogType pType, const std::string& pMessage, const char* pFile, int pLine);

        static std::vector<ILogListener*> mListeners;
    };
}
#define ZP_CORE_INFO(msg)               Zephyrus::Debug::Log::Info(Zephyrus::Debug::Logger::ZP_ZEPHYRUS, msg)
#define ZP_CORE_LOAD(msg)               Zephyrus::Debug::Log::Load(Zephyrus::Debug::Logger::ZP_ZEPHYRUS, msg)
#define ZP_CORE_WARN(msg)               Zephyrus::Debug::Log::Warn(Zephyrus::Debug::Logger::ZP_ZEPHYRUS, msg, __FILE__, __LINE__)
#define ZP_CORE_ERROR(msg)              Zephyrus::Debug::Log::Error(Zephyrus::Debug::Logger::ZP_ZEPHYRUS, msg, __FILE__, __LINE__)
#define ZP_CORE_ASSERT(cond, msg)       Zephyrus::Debug::Log::Assert(Zephyrus::Debug::Logger::ZP_ZEPHYRUS, cond, msg, __FILE__, __LINE__)

#define ZP_EDITOR_INFO(msg)             Zephyrus::Debug::Log::Info(Zephyrus::Debug::Logger::ZP_APP, msg)
#define ZP_EDITOR_LOAD(msg)             Zephyrus::Debug::Log::Load(Zephyrus::Debug::Logger::ZP_APP, msg)
#define ZP_EDITOR_WARN(msg)             Zephyrus::Debug::Log::Warn(Zephyrus::Debug::Logger::ZP_APP, msg, __FILE__, __LINE__)
#define ZP_EDITOR_ERROR(msg)            Zephyrus::Debug::Log::Error(Zephyrus::Debug::Logger::ZP_APP, msg, __FILE__, __LINE__)
#define ZP_EDITOR_ASSERT(cond, msg)     Zephyrus::Debug::Log::Assert(Zephyrus::Debug::Logger::ZP_APP, cond, msg, __FILE__, __LINE__)

#define ZP_INFO(msg)                    Zephyrus::Debug::Log::Info(Zephyrus::Debug::Logger::ZP_GAME, msg)
#define ZP_LOAD(msg)                    Zephyrus::Debug::Log::Load(Zephyrus::Debug::Logger::ZP_GAME, msg)
#define ZP_WARN(msg)                    Zephyrus::Debug::Log::Warn(Zephyrus::Debug::Logger::ZP_GAME, msg, __FILE__, __LINE__)
#define ZP_ERROR(msg)                   Zephyrus::Debug::Log::Error(Zephyrus::Debug::Logger::ZP_GAME, msg, __FILE__, __LINE__)
#define ZP_ASSERT(cond, msg)            Zephyrus::Debug::Log::Assert(Zephyrus::Debug::Logger::ZP_GAME, cond, msg, __FILE__, __LINE__)
