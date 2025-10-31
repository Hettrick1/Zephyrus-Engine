#pragma once

#include "Interface/ITextureBase.h"
#include "IMaterial.h"
#include <map>

namespace Zephyrus::Material
{
	class Material : public IMaterial
	{
	private:
		Render::IShader* mVertShader{ nullptr };
		Render::IShader* mFragShader{ nullptr };
		Render::IShader* mTescShader{ nullptr };
		Render::IShader* mTeseShader{ nullptr };
		Render::IShader* mGeomShader{ nullptr };

		Render::IShaderProgram* mShaderProgram{ nullptr };

		std::map<std::string, Assets::ITextureBase*> mtextures;

		std::map<std::string, int> mIntProperties;
		std::map<std::string, float> mfloatProperties;
		std::map<std::string, Vector2D> mVec2Properties;
		std::map<std::string, Vector3D> mVec3Properties;
		std::map<std::string, Vector4D> mVec4Properties;
		std::map<std::string, Matrix4DRow> mMat4Properties;

	public:

		Material();
		~Material();

		void SetVertexShader(Render::IShader* s) override;
		void SetFragmentShader(Render::IShader* s) override;
		void SetTessControlShader(Render::IShader* s) override;
		void SetTessEvalShader(Render::IShader* s) override;
		void SetGeometryShader(Render::IShader* s) override;

		void RebuildShaderProgram() override;

		void SetTexture(const std::string& uniformName, Assets::ITextureBase* texture) override;
		void RemoveTexture(const std::string& uniformName) override;

		void SetProperty(const std::string& uniformName, float value) override;
		void SetProperty(const std::string& uniformName, int value) override;
		void SetProperty(const std::string& uniformName, const Vector2D& value) override;
		void SetProperty(const std::string& uniformName, const Vector3D& value) override;
		void SetProperty(const std::string& uniformName, const Vector4D& value) override;
		void SetProperty(const std::string& uniformName, const Matrix4DRow& value) override;
		void RemoveProperty(const std::string& uniformName) override;

		void Use() override;

		void Serialize(Serialization::ISerializer& writer) const override;
		void Deserialize(Serialization::IDeserializer& value) override;
	};
}