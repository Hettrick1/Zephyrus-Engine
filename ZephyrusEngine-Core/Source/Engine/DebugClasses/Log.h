#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <string>

enum class LogType {
	INFO,
    LOAD,
	WARN,
	ERROR,
	ASSERT
};

enum class Logger {
    ZEPHYRUS,
    APP
};

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
private:

    static std::string GetTimestamp();

    static std::string GetColorCode(LogType pType);

    static std::string LevelToString(LogType pType);

    static void WriteLog(Logger pLogger, LogType pType, const std::string& pMessage, const char* pFile, int pLine);
};

#define ZP_CORE_INFO(msg)        Log::Info(Logger::ZEPHYRUS, msg)
#define ZP_CORE_LOAD(msg)        Log::Load(Logger::ZEPHYRUS, msg)
#define ZP_CORE_WARN(msg)        Log::Warn(Logger::ZEPHYRUS, msg, __FILE__, __LINE__)
#define ZP_CORE_ERROR(msg)       Log::Error(Logger::ZEPHYRUS, msg, __FILE__, __LINE__)
#define ZP_CORE_ASSERT(cond, msg) Log::Assert(Logger::ZEPHYRUS, cond, msg, __FILE__, __LINE__)

#define ZP_INFO(msg)        Log::Info(Logger::APP, msg)
#define ZP_LOAD(msg)        Log::Load(Logger::APP, msg)
#define ZP_WARN(msg)        Log::Warn(Logger::APP, msg, __FILE__, __LINE__)
#define ZP_ERROR(msg)       Log::Error(Logger::APP, msg, __FILE__, __LINE__)
#define ZP_ASSERT(cond, msg) Log::Assert(Logger::APP, cond, msg, __FILE__, __LINE__)
