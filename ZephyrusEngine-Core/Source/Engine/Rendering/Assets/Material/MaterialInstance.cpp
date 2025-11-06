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

		writer.WriteString("baseMaterial", mBaseMaterial ? mBaseMaterial->GetFilePath() : "");

		if (!mTextureOverrides.empty())
		{
			writer.BeginArray("textures");

			for (auto& [name, tex] : mTextureOverrides)
			{
				writer.BeginObject();

				writer.WriteString("name", name);

				if (auto tex2D = dynamic_cast<Assets::ITexture2D*>(tex))
				{
					writer.WriteString("type", "Texture2D");
					writer.WriteString("path", tex2D->GetFilePath());
				}
				else if (auto cubemap = dynamic_cast<Assets::ICubeMapTexture*>(tex))
				{
					writer.WriteString("type", "Cubemap");
					writer.BeginArray("paths");
					for (auto& facePath : cubemap->GetFaceFilePath())
						writer.PushString(facePath);
					writer.EndArray();
				}
				else
				{
					writer.WriteString("type", "Unknown");
				}

				writer.EndObject();
			}

			writer.EndArray();
		}

		// --- Float properties ---
		if (!mFloatOverrides.empty())
		{
			writer.BeginArray("floatProperties");
			for (auto& [name, value] : mFloatOverrides)
			{
				writer.BeginObject();
				writer.WriteString("name", name);
				writer.WriteFloat("value", value);
				writer.EndObject();
			}
			writer.EndArray();
		}

		// --- Int properties ---
		if (!mIntOverrides.empty())
		{
			writer.BeginArray("intProperties");
			for (auto& [name, value] : mIntOverrides)
			{
				writer.BeginObject();
				writer.WriteString("name", name);
				writer.WriteInt("value", value);
				writer.EndObject();
			}
			writer.EndArray();
		}

		// --- Vec2 properties ---
		if (!mVector2DOverrides.empty())
		{
			writer.BeginArray("vec2Properties");
			for (auto& [name, value] : mVector2DOverrides)
			{
				writer.BeginObject();
				writer.WriteString("name", name);
				writer.WriteVector2D("value", value);
				writer.EndObject();
			}
			writer.EndArray();
		}

		// --- Vec3 properties ---
		if (!mVector3DOverrides.empty())
		{
			writer.BeginArray("vec3Properties");
			for (auto& [name, value] : mVector3DOverrides)
			{
				writer.BeginObject();
				writer.WriteString("name", name);
				writer.WriteVector3D("value", value);
				writer.EndObject();
			}
			writer.EndArray();
		}

		// --- Vec4 properties ---
		if (!mVector4DOverrides.empty())
		{
			writer.BeginArray("vec4Properties");
			for (auto& [name, value] : mVector4DOverrides)
			{
				writer.BeginObject();
				writer.WriteString("name", name);
				writer.WriteVector4D("value", value);
				writer.EndObject();
			}
			writer.EndArray();
		}

		// --- Mat4 properties ---
		if (!mMatrix4DOverrides.empty())
		{
			writer.BeginArray("mat4Properties");
			for (auto& [name, value] : mMatrix4DOverrides)
			{
				writer.BeginObject();
				writer.WriteString("name", name);
				writer.WriteMatrice4DRow("value", value);
				writer.EndObject();
			}
			writer.EndArray();
		}
		writer.EndObject();
	}
	void MaterialInstance::Deserialize(Serialization::IDeserializer& reader)
	{
		// --- Textures ---
		if (reader.BeginObjectArray("textures"))
		{
			while (reader.NextObjectElement())
			{
				auto nameOpt = reader.ReadString("name");
				auto typeOpt = reader.ReadString("type");
				if (!nameOpt.has_value() || !typeOpt.has_value())
					continue;

				const std::string& name = nameOpt.value();
				const std::string& type = typeOpt.value();

				if (type == "Texture2D")
				{
					auto pathOpt = reader.ReadString("path");
					if (pathOpt.has_value())
					{
						auto tex = Assets::AssetsManager::LoadTexture(pathOpt.value(), pathOpt.value());
						if (tex)
							mTextureOverrides[name] = tex;
					}
				}
				else if (type == "Cubemap")
				{
					auto facesOpt = reader.ReadArrayString("paths");
					if (facesOpt.has_value() && facesOpt->size() == 6)
					{
						std::string cubeKey;
						for (auto& path : facesOpt.value())
							cubeKey += path;

						auto tex = Assets::AssetsManager::LoadCubemap(facesOpt.value(), cubeKey);
						if (tex)
							mTextureOverrides[name] = tex;
					}
				}
				else
				{
					ZP_CORE_WARN("Unknown texture type found while deserializing material instance:" + type);
				}
			}

			reader.EndObjectArray();
		}

		// --- Float properties ---
		if (reader.BeginObjectArray("floatProperties"))
		{
			while (reader.NextObjectElement())
			{
				auto nameOpt = reader.ReadString("name");
				auto valueOpt = reader.ReadFloat("value");
				if (nameOpt && valueOpt)
					mFloatOverrides[nameOpt.value()] = valueOpt.value();
			}
			reader.EndObjectArray();
		}

		// --- Int properties ---
		if (reader.BeginObjectArray("intProperties"))
		{
			while (reader.NextObjectElement())
			{
				auto nameOpt = reader.ReadString("name");
				auto valueOpt = reader.ReadInt("value");
				if (nameOpt && valueOpt)
					mIntOverrides[nameOpt.value()] = valueOpt.value();
			}
			reader.EndObjectArray();
		}

		// --- Vec2 properties ---
		if (reader.BeginObjectArray("vec2Properties"))
		{
			while (reader.NextObjectElement())
			{
				auto nameOpt = reader.ReadString("name");
				auto valueOpt = reader.ReadVector2D("value");
				if (nameOpt && valueOpt)
					mVector2DOverrides[nameOpt.value()] = valueOpt.value();
			}
			reader.EndObjectArray();
		}

		// --- Vec3 properties ---
		if (reader.BeginObjectArray("vec3Properties"))
		{
			while (reader.NextObjectElement())
			{
				auto nameOpt = reader.ReadString("name");
				auto valueOpt = reader.ReadVector3D("value");
				if (nameOpt && valueOpt)
					mVector3DOverrides[nameOpt.value()] = valueOpt.value();
			}
			reader.EndObjectArray();
		}

		// --- Vec4 properties ---
		if (reader.BeginObjectArray("vec4Properties"))
		{
			while (reader.NextObjectElement())
			{
				auto nameOpt = reader.ReadString("name");
				auto valueOpt = reader.ReadVector4D("value");
				if (nameOpt && valueOpt)
					mVector4DOverrides[nameOpt.value()] = valueOpt.value();
			}
			reader.EndObjectArray();
		}

		// --- Vec4 properties ---
		if (reader.BeginObjectArray("mat4Properties"))
		{
			while (reader.NextObjectElement())
			{
				auto nameOpt = reader.ReadString("name");
				auto valueOpt = reader.ReadMatrix4DRow("value");
				if (nameOpt && valueOpt)
					mMatrix4DOverrides[nameOpt.value()] = valueOpt.value();
			}
			reader.EndObjectArray();
		}
	}
}