#pragma once
#include <string>

#include "LogEnum.h"

namespace Zephyrus::Debug
{
    class ILogListener
    {
    public:
        virtual ~ILogListener() = default;
        virtual void OnLogMessage(const ZPMessage& pMessage) = 0;
    };
}