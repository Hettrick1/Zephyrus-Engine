#include "MaterialInstance.h"

namespace Zephyrus::Material
{
	MaterialInstance::MaterialInstance(IMaterial* base)
		: mBaseMaterial(base)
	{
	}
	void MaterialInstance::SetMaterial(IMaterial* newMaterial)
	{
		mBaseMaterial = newMaterial;
		ClearOverrides();
	}
	void MaterialInstance::ClearOverride(const std::string& name)
	{
		mFloatOverrides.erase(name);
		mIntOverrides.erase(name);
		mVector2DOverrides.erase(name);
		mVector3DOverrides.erase(name);
		mVector4DOverrides.erase(name);
		mMatrix4DOverrides.erase(name);
		mTextureOverrides.erase(name);
	}
	void MaterialInstance::ClearOverrides()
	{
		mFloatOverrides.clear();
		mIntOverrides.clear();
		mVector2DOverrides.clear();
		mVector3DOverrides.clear();
		mVector4DOverrides.clear();
		mMatrix4DOverrides.clear();
		mTextureOverrides.clear();
	}
	void MaterialInstance::SetFloat(const std::string& name, float value)
	{
		mFloatOverrides[name] = value;
	}
	void MaterialInstance::SetInt(const std::string& name, int value)
	{
		mIntOverrides[name] = value;
	}
	void MaterialInstance::SetVector2D(const std::string& name, const Vector2D& value)
	{
		mVector2DOverrides[name] = value;
	}
	void MaterialInstance::SetVector3D(const std::string& name, const Vector3D& value)
	{
		mVector3DOverrides[name] = value;
	}
	void MaterialInstance::SetVector4D(const std::string& name, const Vector4D& value)
	{
		mVector4DOverrides[name] = value;
	}
	void MaterialInstance::SetMatrix4DRow(const std::string& name, const Matrix4DRow& value)
	{
		mMatrix4DOverrides[name] = value;
	}
	void MaterialInstance::SetTexture(const std::string& name, Assets::ITextureBase* texture)
	{
		mTextureOverrides[name] = texture;
	}
	void MaterialInstance::Use(const Matrix4DRow* world, const Matrix4DRow* viewproj)
	{
		if (!mBaseMaterial) return;
		mBaseMaterial->Use(world, viewproj);

		auto shaderProgram = mBaseMaterial->GetShaderProgram();

		for (auto& [name, value] : mFloatOverrides)
		{
			shaderProgram->setFloat(name.c_str(), value);
		}
		for (auto& [name, value] : mIntOverrides)
		{
			shaderProgram->setInteger(name.c_str(), value);
		}
		for (auto& [name, value] : mVector2DOverrides)
		{
			shaderProgram->setVector2f(name.c_str(), value);
		}
		for (auto& [name, value] : mVector3DOverrides)
		{
			shaderProgram->setVector3f(name.c_str(), value);
		}
		for (auto& [name, value] : mVector4DOverrides)
		{
			shaderProgram->setVector4f(name.c_str(), value);
		}
		for (auto& [name, value] : mMatrix4DOverrides)
		{
			shaderProgram->setMatrix4Row(name.c_str(), value);
		}

		int slot = 0;
		for (auto& [name, tex] : mTextureOverrides)
		{
			tex->Bind(slot);
			++slot;
		}
	}
	void MaterialInstance::Serialize(Serialization::ISerializer& writer) const
	{
	}
	void MaterialInstance::Deserialize(Serialization::IDeserializer& reader)
	{
	}
}