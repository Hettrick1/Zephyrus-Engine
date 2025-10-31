#include "Material.h"

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

		mtextures.clear();

		mIntProperties.clear();
		mfloatProperties.clear();
		mVec2Properties.clear();
		mVec3Properties.clear();
		mVec4Properties.clear();
		mMat4Properties.clear();
	}
	void Material::SetVertexShader(Render::IShader* s)
	{
		mVertShader = s;
		RebuildShaderProgram();
	}
	void Material::SetFragmentShader(Render::IShader* s)
	{
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

	}
	void Material::SetTexture(const std::string& uniformName, Assets::ITextureBase* texture)
	{
	}
	void Material::RemoveTexture(const std::string& uniformName)
	{
	}
	void Material::SetProperty(const std::string& uniformName, float value)
	{
	}
	void Material::SetProperty(const std::string& uniformName, int value)
	{
	}
	void Material::SetProperty(const std::string& uniformName, const Vector2D& value)
	{
	}
	void Material::SetProperty(const std::string& uniformName, const Vector3D& value)
	{
	}
	void Material::SetProperty(const std::string& uniformName, const Vector4D& value)
	{
	}
	void Material::SetProperty(const std::string& uniformName, const Matrix4DRow& value)
	{
	}
	void Material::RemoveProperty(const std::string& uniformName)
	{
	}
	void Material::Use()
	{
	}
	void Material::Serialize(Serialization::ISerializer& writer) const
	{
	}
	void Material::Deserialize(Serialization::IDeserializer& value)
	{
	}
}