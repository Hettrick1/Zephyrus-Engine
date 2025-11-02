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
		shaders.push_back(mFragShader);
		if (mTescShader && mTeseShader)
		{
			shaders.push_back(mTescShader);
			shaders.push_back(mTeseShader);
		}
		if (mGeomShader)
		{
			shaders.push_back(mGeomShader);
		}

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
	void Material::Use()
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
	}
	void Material::Use(const Matrix4DRow& world, const Matrix4DRow& viewProj)
	{
		Use();
		mShaderProgram->setMatrix4Row("uViewProj", viewProj);
		mShaderProgram->setMatrix4Row("uWorldTransform", world);
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

		writer.BeginObject("textures");

		for (auto& [name, tex] : mTextures)
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

		writer.BeginObject("floatProperties");
		for (auto& [name, value] : mfloatProperties)
			writer.WriteFloat(name.c_str(), value);
		writer.EndObject();

		writer.BeginObject("intProperties");
		for (auto& [name, value] : mIntProperties)
			writer.WriteInt(name.c_str(), value);
		writer.EndObject();

		writer.BeginObject("vec2Properties");
		for (auto& [name, value] : mVec2Properties)
			writer.WriteVector2D(name.c_str(), value);
		writer.EndObject();

		writer.BeginObject("vec3Properties");
		for (auto& [name, value] : mVec3Properties)
			writer.WriteVector3D(name.c_str(), value);
		writer.EndObject();

		writer.BeginObject("vec4Properties");
		for (auto& [name, value] : mVec4Properties)
			writer.WriteVector4D(name.c_str(), value);
		writer.EndObject();

		writer.BeginObject("mat4Properties");
		for (auto& [name, value] : mMat4Properties)
			writer.WriteMatrice4DRow(name.c_str(), value);
		writer.EndObject();
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
							mTextures[reader.GetCurrentKey()] = tex;
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
							mTextures[reader.GetCurrentKey()] = tex;
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
					mfloatProperties[key] = value.value();
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
					mIntProperties[key] = value.value();
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
					mVec2Properties[key] = value.value();
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
					mVec3Properties[key] = value.value();
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
					mVec4Properties[key] = value.value();
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
					mMat4Properties[key] = matOpt.value();
			}
			reader.EndObject();
		}
		RebuildShaderProgram();
	}
}