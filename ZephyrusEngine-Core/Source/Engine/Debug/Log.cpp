#include "Log.h"

namespace Zephyrus::Debug
{
    std::vector<ILogListener*> Log::mListeners;

    void Log::Init()
    {
    }

    void Log::Shutdown()
    {
        mListeners.clear();
    }

    void Log::Info(Logger pLogger, const std::string& pMessage)
    {
        WriteLog(pLogger, LogType::ZLT_INFO, pMessage, "", 0);
    }

    void Log::Load(Logger pLogger, const std::string& pMessage)
    {
        WriteLog(pLogger, LogType::ZLT_LOAD, pMessage, "", 0);
    }

    void Log::Warn(Logger pLogger, const std::string& pMessage, const char* pFile, int pLine)
    {
        WriteLog(pLogger, LogType::ZLT_WARN, pMessage, pFile, pLine);
    }

    void Log::Error(Logger pLogger, const std::string& pMessage, const char* pFile, int pLine)
    {
        WriteLog(pLogger, LogType::ZLT_ERROR, pMessage, pFile, pLine);
    }

    void Log::Assert(Logger pLogger, bool pCondition, const std::string& pMessage, const char* pFile, int pLine)
    {
        if (!pCondition) {
            WriteLog(pLogger, LogType::ZLT_ASSERT, pMessage, pFile, pLine);
            abort();
        }
    }

    void Log::AddListener(ILogListener* listener)
    {
        mListeners.push_back(listener);
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
        case LogType::ZLT_INFO:   return "\033[0m";             // White
        case LogType::ZLT_LOAD:   return "\033[38;5;152m";      // Green
        case LogType::ZLT_WARN:   return "\033[38;5;220m";            // Yellow
        case LogType::ZLT_ERROR:  return "\033[31m";            // Red
        case LogType::ZLT_ASSERT: return "\033[1;31m";          // Dark Red
        default:               return "\033[0m";
        }
    }

    std::string Log::LevelToString(LogType level)
    {
        switch (level) {
        case LogType::ZLT_INFO:   return "INFO";
        case LogType::ZLT_LOAD:   return "LOAD";
        case LogType::ZLT_WARN:   return "WARN";
        case LogType::ZLT_ERROR:  return "ERROR";
        case LogType::ZLT_ASSERT: return "ASSERT";
        default:               return "UNKNOWN";
        }
    }

    void Log::WriteLog(Logger logger, LogType level, const std::string& message, const char* file, int line)
    {
        std::ostringstream output;
        output << "[" << GetTimestamp() << "] ";
        switch (logger)
        {
        case Logger::ZP_ZEPHYRUS:
            output << "[ ZEPHYRUS - ";
            break;
        case Logger::ZP_APP:
            output << "[ EDITOR - ";
            break;
        case Logger::ZP_GAME:
            output << "[ GAME - ";
            break;
        default:
            break;
        }
        output << LevelToString(level) << "] ";

        if (level != LogType::ZLT_INFO && file != nullptr && line != 0) {
            output << "[" << file << ":" << line << "] ";
        }

        output << message;
        if (!mListeners.empty())
        {
            for (auto* listener : mListeners)
            {
                ZPMessage message;
                message.pTexte = output.str();
                message.pLogger = logger;
                message.pType = level;

                listener->OnLogMessage(message);
            }
        }
        std::cout << GetColorCode(level) << output.str() << "\033[0m" << std::endl;
    }
}
