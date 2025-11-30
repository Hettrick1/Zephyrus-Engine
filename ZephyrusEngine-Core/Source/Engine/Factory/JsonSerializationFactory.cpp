#include "pch.h"
#include "JsonSerializationFactory.h"

std::unique_ptr<Serialization::IDeserializer> Zephyrus::Factory::JsonSerializationFactory::CreateDeserializer()
{
    return std::make_unique<Serialization::Json::JsonReader>();
}

std::unique_ptr<Serialization::ISerializer> Zephyrus::Factory::JsonSerializationFactory::CreateSerializer()
{
    return std::make_unique<Serialization::Json::JsonWriter>();
}
