#include "Log.h"

void Log::Init()
{
}

void Log::Shutdown()
{
}

void Log::Info(Logger pLogger, const std::string& pMessage)
{
    WriteLog(pLogger, LogType::INFO, pMessage, "", 0);
}

void Log::Load(Logger pLogger, const std::string& pMessage)
{
    WriteLog(pLogger, LogType::LOAD, pMessage, "", 0);
}

void Log::Warn(Logger pLogger, const std::string& pMessage, const char* pFile, int pLine)
{
    WriteLog(pLogger, LogType::WARN, pMessage, pFile, pLine);
}

void Log::Error(Logger pLogger, const std::string& pMessage, const char* pFile, int pLine)
{
    WriteLog(pLogger, LogType::ERROR, pMessage, pFile, pLine);
}

void Log::Assert(Logger pLogger, bool pCondition, const std::string& pMessage, const char* pFile, int pLine)
{
    if (!pCondition) {
        WriteLog(pLogger, LogType::ASSERT, pMessage, pFile, pLine);
        abort();
    }
}

std::string Log::GetTimestamp()
{
    std::time_t t = std::time(nullptr); 
    std::tm tm_safe; 
    localtime_s(&tm_safe, &t);
    std::ostringstream oss;
    oss << std::put_time(&tm_safe, "%H:%M:%S");
    return oss.str();
}

std::string Log::GetColorCode(LogType level)
{
    switch (level) {
    case LogType::INFO:   return "\033[0m";             // White
    case LogType::LOAD:   return "\033[38;5;152m";      // Green
    case LogType::WARN:   return "\033[38;5;220m";            // Yellow
    case LogType::ERROR:  return "\033[31m";            // Red
    case LogType::ASSERT: return "\033[1;31m";          // Dark Red
    default:               return "\033[0m";
    }
}

std::string Log::LevelToString(LogType level)
{
    switch (level) {
    case LogType::INFO:   return "INFO";
    case LogType::LOAD:   return "LOAD";
    case LogType::WARN:   return "WARN";
    case LogType::ERROR:  return "ERROR";
    case LogType::ASSERT: return "ASSERT";
    default:               return "UNKNOWN";
    }
}

void Log::WriteLog(Logger logger, LogType level, const std::string& message, const char* file, int line)
{
    std::ostringstream output;
    output << "[" << GetTimestamp() << "] ";
    switch (logger)
    {
    case Logger::ZEPHYRUS:
        output << "[ ZEPHYRUS - ";
        break;
    case Logger::APP:
        output << "[ APP - ";
        break;
    default:
        break;
    }
    output << LevelToString(level) << "] ";

    if (level != LogType::INFO && file != nullptr && line != 0) {
        output << "[" << file << ":" << line << "] ";
    }

    output << message;

    std::cout << GetColorCode(level) << output.str() << "\033[0m" << std::endl;
}

