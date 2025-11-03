#pragma once
#include "ISerializationFactory.h"
#include "JSONUtils.h"

namespace Zephyrus::Factory
{
    class JsonSerializationFactory : public ISerializationFactory
    {
    public:
        std::unique_ptr<Serialization::IDeserializer> CreateDeserializer() override;
        std::unique_ptr<Serialization::ISerializer> CreateSerializer() override;
    };
}