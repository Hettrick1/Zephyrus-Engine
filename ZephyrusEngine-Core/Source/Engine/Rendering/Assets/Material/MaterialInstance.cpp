#include "MaterialInstance.h"
#include "Interface/ITexture2D.h"
#include "Interface/ICubeMapTexture.h"
#include "Assets.h"

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
		writer.BeginObject("materialInstance");

		writer.WriteString("baseMaterial", mBaseMaterial->GetFilePath());

		writer.BeginObject("textures");
		for (auto& [name, tex] : mTextureOverrides)
		{
			writer.BeginObject(name.c_str());

			if (auto tex2D = dynamic_cast<Assets::ITexture2D*>(tex))
			{
				writer.WriteString("type", "Texture2D");
				writer.WriteString("path", tex2D->GetFilePath());
			}
			else if (auto cubemap = dynamic_cast<Assets::ICubeMapTexture*>(tex))
			{
				writer.WriteString("type", "Cubemap");
				writer.BeginArray("paths");
				for (auto& face : cubemap->GetFaceFilePath())
					writer.PushString(face);
				writer.EndArray();
			}
			else
			{
				writer.WriteString("type", "Unknown");
			}

			writer.EndObject();
		}

		writer.EndObject();

		if (!mFloatOverrides.empty())
		{
			writer.BeginObject("floatProperties");
			for (auto& [name, value] : mFloatOverrides)
				writer.WriteFloat(name.c_str(), value);
			writer.EndObject();
		}

		if (!mIntOverrides.empty())
		{
			writer.BeginObject("intProperties");
			for (auto& [name, value] : mIntOverrides)
				writer.WriteInt(name.c_str(), value);
			writer.EndObject();
		}

		if (!mVector2DOverrides.empty())
		{
			writer.BeginObject("vec2Properties");
			for (auto& [name, value] : mVector2DOverrides)
				writer.WriteVector2D(name.c_str(), value);
			writer.EndObject();
		}

		if (!mVector3DOverrides.empty())
		{
			writer.BeginObject("vec3Properties");
			for (auto& [name, value] : mVector3DOverrides)
				writer.WriteVector3D(name.c_str(), value);
			writer.EndObject();
		}

		if (!mVector4DOverrides.empty())
		{
			writer.BeginObject("vec4Properties");
			for (auto& [name, value] : mVector4DOverrides)
				writer.WriteVector4D(name.c_str(), value);
			writer.EndObject();
		}

		if (!mMatrix4DOverrides.empty())
		{
			writer.BeginObject("mat4Properties");
			for (auto& [name, value] : mMatrix4DOverrides)
				writer.WriteMatrice4DRow(name.c_str(), value);
			writer.EndObject();
		}
		writer.EndObject();
	}
	void MaterialInstance::Deserialize(Serialization::IDeserializer& reader)
	{
		if (reader.BeginObject("textures"))
		{
			while (reader.NextObjectElement())
			{
				auto texTypeOpt = reader.ReadString("type");
				if (!texTypeOpt.has_value())
					continue;

				const std::string& type = texTypeOpt.value();

				if (type == "Texture2D")
				{
					auto pathOpt = reader.ReadString("path");
					if (pathOpt.has_value())
					{
						auto tex = Assets::AssetsManager::LoadTexture(pathOpt.value(), pathOpt.value());
						if (tex)
							mTextureOverrides[reader.GetCurrentKey()] = tex;
					}
				}
				else if (type == "Cubemap")
				{
					auto facesOpt = reader.ReadArrayString("paths");
					if (facesOpt.has_value() && facesOpt.value().size() == 6)
					{
						std::string name;
						for (auto path : facesOpt.value())
						{
							name += path;
						}
						auto tex = Assets::AssetsManager::LoadCubemap(facesOpt.value(), name);
						if (tex)
						{
							mTextureOverrides[reader.GetCurrentKey()] = tex;
						}
					}
				}
				else
				{
					ZP_CORE_WARN("Unknown texture type found in material deserialization");
				}
			}

			reader.EndObject();
		}

		if (reader.BeginObject("floatProperties"))
		{
			while (reader.NextObjectElement())
			{
				std::string key = reader.GetCurrentKey();
				auto value = reader.ReadFloat();
				if (value.has_value())
					mFloatOverrides[key] = value.value();
			}
			reader.EndObject();
		}

		if (reader.BeginObject("intProperties"))
		{
			while (reader.NextObjectElement())
			{
				std::string key = reader.GetCurrentKey();
				auto value = reader.ReadInt();
				if (value.has_value())
					mIntOverrides[key] = value.value();
			}
			reader.EndObject();
		}

		if (reader.BeginObject("vec2Properties"))
		{
			while (reader.NextObjectElement())
			{
				std::string key = reader.GetCurrentKey();
				auto value = reader.ReadVector2D();
				if (value.has_value())
					mVector2DOverrides[key] = value.value();
			}
			reader.EndObject();
		}

		if (reader.BeginObject("vec3Properties"))
		{
			while (reader.NextObjectElement())
			{
				std::string key = reader.GetCurrentKey();
				auto value = reader.ReadVector3D();
				if (value.has_value())
					mVector3DOverrides[key] = value.value();
			}
			reader.EndObject();
		}

		if (reader.BeginObject("vec4Properties"))
		{
			while (reader.NextObjectElement())
			{
				std::string key = reader.GetCurrentKey();
				auto value = reader.ReadVector4D();
				if (value.has_value())
					mVector4DOverrides[key] = value.value();
			}
			reader.EndObject();
		}

		if (reader.BeginObject("mat4Properties"))
		{
			while (reader.NextObjectElement())
			{
				std::string key = reader.GetCurrentKey();
				auto matOpt = reader.ReadMatrix4DRow();
				if (matOpt.has_value())
					mMatrix4DOverrides[key] = matOpt.value();
			}
			reader.EndObject();
		}
	}
}