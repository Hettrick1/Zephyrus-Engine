#pragma once

namespace Zephyrus::Debug
{
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

    struct ZPMessage
    {
        std::string pTexte;
        LogType pType;
        Logger pLogger;
    };

}