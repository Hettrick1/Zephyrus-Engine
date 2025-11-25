#include "Material.h"

#include "Assets.h"
#include "Interface/ICubeMapTexture.h"
#include "Interface/ITexture2D.h"

namespace Zephyrus::Material
{
	Material::Material()
	{
	}
	Material::~Material()
	{
		mVertShader = nullptr;
		mFragShader = nullptr;
		mTescShader = nullptr;
		mTeseShader = nullptr;
		mGeomShader = nullptr;

		mShaderProgram = nullptr;

		mTextures.clear();

		mIntProperties.clear();
		mfloatProperties.clear();
		mVec2Properties.clear();
		mVec3Properties.clear();
		mVec4Properties.clear();
		mMat4Properties.clear();
	}
	void Material::SetVertexShader(Render::IShader* s)
	{
		if (!s) return;
		mVertShader = s;
		RebuildShaderProgram();
	}
	void Material::SetFragmentShader(Render::IShader* s)
	{
		if (!s) return;
		mFragShader = s;
		RebuildShaderProgram();
	}
	void Material::SetTessControlShader(Render::IShader* s)
	{
		mTescShader = s;
		RebuildShaderProgram();
	}
	void Material::SetTessEvalShader(Render::IShader* s)
	{
		mTeseShader = s;
		RebuildShaderProgram();
	}
	void Material::SetGeometryShader(Render::IShader* s)
	{
		mGeomShader = s;
		RebuildShaderProgram();
	}
	void Material::RebuildShaderProgram()
	{
		std::vector<Render::IShader*> shaders;

		if (!mVertShader || !mFragShader)
		{
			ZP_CORE_ERROR("You need a vertex and a fragment shader !");
			return;
		}
		shaders.push_back(mVertShader);
		if (mTescShader && mTeseShader)
		{
			shaders.push_back(mTescShader);
			shaders.push_back(mTeseShader);
		}
		if (mGeomShader)
		{
			shaders.push_back(mGeomShader);
		}
		shaders.push_back(mFragShader);

		std::string programName;
		for (auto s : shaders)
		{
			programName += s->GetFilePath();
		}

		mShaderProgram = Assets::AssetsManager::LoadShaderProgram(shaders, programName);

	}
	void Material::SetTexture(const std::string& uniformName, Assets::ITextureBase* texture)
	{
		if (!texture) return;

		mTextures[uniformName] = texture;

	}
	void Material::RemoveTexture(const std::string& uniformName)
	{
		mTextures.erase(uniformName);
	}
	void Material::SetProperty(const std::string& uniformName, float value)
	{
		mfloatProperties[uniformName] = value;
	}
	void Material::SetProperty(const std::string& uniformName, int value)
	{
		mIntProperties[uniformName] = value;
	}
	void Material::SetProperty(const std::string& uniformName, const Vector2D& value)
	{
		mVec2Properties[uniformName] = value;
	}
	void Material::SetProperty(const std::string& uniformName, const Vector3D& value)
	{
		mVec3Properties[uniformName] = value;
	}
	void Material::SetProperty(const std::string& uniformName, const Vector4D& value)
	{
		mVec4Properties[uniformName] = value;
	}
	void Material::SetProperty(const std::string& uniformName, const Matrix4DRow& value)
	{
		mMat4Properties[uniformName] = value;
	}
	void Material::RemoveProperty(const std::string& uniformName)
	{
		mIntProperties.erase(uniformName);
		mfloatProperties.erase(uniformName);
		mVec2Properties.erase(uniformName);
		mVec3Properties.erase(uniformName);
		mVec4Properties.erase(uniformName);
		mMat4Properties.erase(uniformName);
	}
	void Material::Use(const Matrix4DRow* world, const Matrix4DRow* viewproj)
	{
		if (!mShaderProgram)
			return;

		mShaderProgram->Use();
		
		for (auto& [name, value] : mfloatProperties)
		{
			mShaderProgram->setFloat(name.c_str(), value);
		}
		for (auto& [name, value] : mIntProperties)
		{
			mShaderProgram->setInteger(name.c_str(), value);
		}
		for (auto& [name, value] : mVec2Properties)
		{
			mShaderProgram->setVector2f(name.c_str(), value);
		}
		for (auto& [name, value] : mVec3Properties)
		{
			mShaderProgram->setVector3f(name.c_str(), value);
		}
		for (auto& [name, value] : mVec4Properties)
		{
			mShaderProgram->setVector4f(name.c_str(), value);
		}
		for (auto& [name, value] : mMat4Properties)
		{
			mShaderProgram->setMatrix4Row(name.c_str(), value);
		}

		int slot = 0;
		for (auto& [name, tex] : mTextures)
		{
			tex->Bind(slot);
			++slot;
		}
		if (world)
		{
			mShaderProgram->setMatrix4Row("uWorldTransform", *world);
		}
		if (viewproj)
		{
			mShaderProgram->setMatrix4Row("uViewProj", *viewproj);
		}
	}

	void Material::Serialize(Serialization::ISerializer& writer) const
	{
		writer.BeginObject("shaders");
		if (mVertShader)
		{
			writer.WriteString("vertex", mVertShader->GetFilePath());
		}
		if (mFragShader)
		{
			writer.WriteString("fragment", mFragShader->GetFilePath());
		}
		if (mTescShader)
		{
			writer.WriteString("control", mTescShader->GetFilePath());
		}
		if (mTeseShader)
		{
			writer.WriteString("eval", mTeseShader->GetFilePath());
		}
		if (mGeomShader)
		{
			writer.WriteString("geometry", mGeomShader->GetFilePath());
		}
		writer.EndObject();

		if (!mTextures.empty())
		{
			writer.BeginArray("textures");

			for (auto& [name, tex] : mTextures)
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
		if (!mfloatProperties.empty())
		{
			writer.BeginArray("floatProperties");
			for (auto& [name, value] : mfloatProperties)
			{
				writer.BeginObject();
				writer.WriteString("name", name);
				writer.WriteFloat("value", value);
				writer.EndObject();
			}
			writer.EndArray();
		}

		// --- Int properties ---
		if (!mIntProperties.empty())
		{
			writer.BeginArray("intProperties");
			for (auto& [name, value] : mIntProperties)
			{
				writer.BeginObject();
				writer.WriteString("name", name);
				writer.WriteInt("value", value);
				writer.EndObject();
			}
			writer.EndArray();
		}

		// --- Vec2 properties ---
		if (!mVec2Properties.empty())
		{
			writer.BeginArray("vec2Properties");
			for (auto& [name, value] : mVec2Properties)
			{
				writer.BeginObject();
				writer.WriteString("name", name);
				writer.WriteVector2D("value", value);
				writer.EndObject();
			}
			writer.EndArray();
		}

		// --- Vec3 properties ---
		if (!mVec3Properties.empty())
		{
			writer.BeginArray("vec3Properties");
			for (auto& [name, value] : mVec3Properties)
			{
				writer.BeginObject();
				writer.WriteString("name", name);
				writer.WriteVector3D("value", value);
				writer.EndObject();
			}
			writer.EndArray();
		}

		// --- Vec4 properties ---
		if (!mVec4Properties.empty())
		{
			writer.BeginArray("vec4Properties");
			for (auto& [name, value] : mVec4Properties)
			{
				writer.BeginObject();
				writer.WriteString("name", name);
				writer.WriteVector4D("value", value);
				writer.EndObject();
			}
			writer.EndArray();
		}

		// --- Mat4 properties ---
		if (!mMat4Properties.empty())
		{
			writer.BeginArray("mat4Properties");
			for (auto& [name, value] : mMat4Properties)
			{
				writer.BeginObject();
				writer.WriteString("name", name);
				writer.WriteMatrice4DRow("value", value);
				writer.EndObject();
			}
			writer.EndArray();
		}
	}
	void Material::Deserialize(Serialization::IDeserializer& reader)
	{
		if (reader.BeginObject("shaders"))
		{
			if (auto vertex = reader.ReadString("vertex"))
			{
				mVertShader = Assets::AssetsManager::LoadShader(*vertex, Render::ShaderType::VERTEX, *vertex);
			}
			if (auto fragment = reader.ReadString("fragment"))
			{
				mFragShader = Assets::AssetsManager::LoadShader(*fragment, Render::ShaderType::FRAGMENT, *fragment);
			}
			if (auto control = reader.ReadString("control"))
			{
				mTescShader = Assets::AssetsManager::LoadShader(*control, Render::ShaderType::TESSELLATION_CONTROL, *control);
			}
			if (auto eval = reader.ReadString("eval"))
			{
				mTeseShader = Assets::AssetsManager::LoadShader(*eval, Render::ShaderType::TESSELLATION_EVALUATION, *eval);
			}
			if (auto geometry = reader.ReadString("geometry"))
			{
				mGeomShader = Assets::AssetsManager::LoadShader(*geometry, Render::ShaderType::GEOMETRY, *geometry);
			}
			reader.EndObject();
		}

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
							mTextures[name] = tex;
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
							mTextures[name] = tex;
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
					mfloatProperties[nameOpt.value()] = valueOpt.value();
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
					mIntProperties[nameOpt.value()] = valueOpt.value();
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
					mVec2Properties[nameOpt.value()] = valueOpt.value();
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
					mVec3Properties[nameOpt.value()] = valueOpt.value();
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
					mVec4Properties[nameOpt.value()] = valueOpt.value();
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
					mMat4Properties[nameOpt.value()] = valueOpt.value();
			}
			reader.EndObjectArray();
		}
		RebuildShaderProgram();
	}

	std::vector<PropertyDescriptor> Material::GetProperties()
	{
		return
		{
			{"Vertex Shader : ", &mVertShader, PropertyType::ShaderVert},
			{"Fragment Shader : ", &mFragShader, PropertyType::ShaderFrag},
			{"Tesselation Control Shader : ", &mTescShader, PropertyType::ShaderTesc},
			{"Tesselation Evaluation Shader : ", &mTeseShader, PropertyType::ShaderTese},
			{"Geometry Shader : ", &mGeomShader, PropertyType::ShaderGeom},
			{"", &mIsAlbedoTexture, PropertyType::Bool, DropDown("Texture","Vector 3D")},
			{"Albedo : ", &mAlbedoTexture, PropertyType::TextureBase, Condition(mIsAlbedoTexture, PropertyFlags::Hidden_In_Editor)},
			{"Albedo : ", &mAlbedoColor, PropertyType::Color, Condition(!mIsAlbedoTexture, PropertyFlags::Hidden_In_Editor)},
			{"", &mIsMetallicTexture, PropertyType::Bool,DropDown("Texture","Float")},
			{"Metallic : ", &mMetallicTexture, PropertyType::TextureBase, Condition(mIsMetallicTexture, PropertyFlags::Hidden_In_Editor)},
			{"Metallic : ", &mMetallic, PropertyType::Float, Condition(!mIsMetallicTexture, PropertyFlags::Hidden_In_Editor) | Range(0.0f, 1.0f)},
			{"", &mIsRoughnessTexture, PropertyType::Bool, DropDown("Texture","Float")},
			{"Roughness : ", &mRoughnessTexture, PropertyType::TextureBase, Condition(mIsRoughnessTexture, PropertyFlags::Hidden_In_Editor)},
			{"Roughness : ", &mRoughness, PropertyType::Float, Condition(!mIsRoughnessTexture, PropertyFlags::Hidden_In_Editor)| Range(0.0f, 1.0f)},
			{"", &mIsSpecularTexture, PropertyType::Bool, DropDown("Texture","Float")},
			{"Specular : ", &mSpecularTexture, PropertyType::TextureBase, Condition(mIsSpecularTexture, PropertyFlags::Hidden_In_Editor)},
			{"Specular : ", &mSpecular, PropertyType::Float, Condition(!mIsSpecularTexture, PropertyFlags::Hidden_In_Editor)| Range(0.0f, 1.0f)},
			
		};
	}

	void Material::SetFilePath(const std::string& filePath)
	{
		mFilePath = filePath;
	}
}