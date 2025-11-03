#pragma once

#include "IMaterial.h"

namespace Zephyrus::Material
{
	class IMaterialInstance
	{
	public:
		virtual ~IMaterialInstance() = default;

		virtual void SetMaterial(IMaterial* newMaterial) = 0;
		virtual void ClearOverride(const std::string& name) = 0;
		virtual void ClearOverrides() = 0;

		virtual void SetFloat(const std::string& name, float value) = 0;
		virtual void SetInt(const std::string& name, int value) = 0;
		virtual void SetVector2D(const std::string& name, const Vector2D& value) = 0;
		virtual void SetVector3D(const std::string& name, const Vector3D& value) = 0;
		virtual void SetVector4D(const std::string& name, const Vector4D& value) = 0;
		virtual void SetMatrix4DRow(const std::string& name, const Matrix4DRow& value) = 0;
		virtual void SetTexture(const std::string& name, Assets::ITextureBase* texture) = 0;

		virtual void Use(const Matrix4DRow* world = nullptr, const Matrix4DRow* viewproj = nullptr) = 0;

		virtual void Serialize(Serialization::ISerializer& writer) const = 0;
		virtual void Deserialize(Serialization::IDeserializer& reader) = 0;

		virtual IMaterial* GetBaseMaterial() const = 0;
	};
}