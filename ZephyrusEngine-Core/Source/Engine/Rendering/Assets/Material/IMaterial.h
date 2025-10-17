#pragma once

#include <string>

#include "JSONUtils.h"

namespace Zephyrus::Material
{
    class IMaterial
    {
    public:
        virtual ~IMaterial() = default;

        // Binding du material pour le renderer actuel
        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        // Accès aux textures par layer
        virtual void SetTexture(const std::string& layerName, const std::string& texturePath) = 0;
        virtual const std::string& GetTexture(const std::string& layerName) const = 0;

        // Paramètres PBR
        virtual void SetParameter(const std::string& name, float value) = 0;
        virtual float GetParameter(const std::string& name) const = 0;

        // Sérialisation / désérialisation
        virtual void Serialize(Serialization::ISerializer& writer) const = 0;
        virtual void Deserialize(Serialization::IDeserializer&& value) = 0;
    };
}