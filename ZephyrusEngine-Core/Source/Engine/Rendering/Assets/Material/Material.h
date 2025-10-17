#pragma once

#include "Texture.h"
#include "IMaterial.h"

namespace Zephyrus::Material
{
	class Material : public IMaterial
	{
	public:
		Material();
		~Material();

        void Bind() const;
        void Unbind() const;

        // Accès aux textures par layer
        void SetTexture(const std::string& layerName, const std::string& texturePath);
        const std::string& GetTexture(const std::string& layerName) const;

        // Paramètres PBR
        void SetParameter(const std::string& name, float value);
        float GetParameter(const std::string& name) const;

        // Sérialisation / désérialisation
        void Serialize(Serialization::ISerializer& writer) const;
        void Deserialize(Serialization::IDeserializer&& value);
	private:
        std::unordered_map<std::string, Assets::Texture*> mTextures;
        std::unordered_map<std::string, float> mParams;
	};
}