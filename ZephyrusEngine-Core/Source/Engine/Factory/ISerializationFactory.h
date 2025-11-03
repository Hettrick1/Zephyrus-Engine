#pragma once
#include "IDeserializer.h"
#include "ISerializer.h"
#include <memory>

namespace Zephyrus::Factory
{
    class ISerializationFactory
    {
    public:
        virtual ~ISerializationFactory() = default;
        virtual std::unique_ptr<Serialization::IDeserializer> CreateDeserializer() = 0;
        virtual std::unique_ptr<Serialization::ISerializer> CreateSerializer() = 0;
    };
}