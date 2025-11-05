#pragma once

#include "Interface/ITextureBase.h"
#include "IMaterial.h"
#include <unordered_map>

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

		std::unordered_map<std::string, Assets::ITextureBase*> mTextures;

		std::unordered_map<std::string, int> mIntProperties;
		std::unordered_map<std::string, float> mfloatProperties;
		std::unordered_map<std::string, Vector2D> mVec2Properties;
		std::unordered_map<std::string, Vector3D> mVec3Properties;
		std::unordered_map<std::string, Vector4D> mVec4Properties;
		std::unordered_map<std::string, Matrix4DRow> mMat4Properties;

		std::string mFilePath{ " " };

	public:

		Material();
		~Material();

		void SetVertexShader(Render::IShader* s) override;
		void SetFragmentShader(Render::IShader* s) override;
		void SetTessControlShader(Render::IShader* s) override;
		void SetTessEvalShader(Render::IShader* s) override;
		void SetGeometryShader(Render::IShader* s) override;

		void RebuildShaderProgram() override;

		inline Render::IShaderProgram* GetShaderProgram() override { return mShaderProgram; }

		void SetTexture(const std::string& uniformName, Assets::ITextureBase* texture) override;
		void RemoveTexture(const std::string& uniformName) override;

		void SetProperty(const std::string& uniformName, float value) override;
		void SetProperty(const std::string& uniformName, int value) override;
		void SetProperty(const std::string& uniformName, const Vector2D& value) override;
		void SetProperty(const std::string& uniformName, const Vector3D& value) override;
		void SetProperty(const std::string& uniformName, const Vector4D& value) override;
		void SetProperty(const std::string& uniformName, const Matrix4DRow& value) override;
		void RemoveProperty(const std::string& uniformName) override;

		void Use(const Matrix4DRow* world = nullptr, const Matrix4DRow* viewproj = nullptr) override;

		void Serialize(Serialization::ISerializer& writer) const override;
		void Deserialize(Serialization::IDeserializer& reader) override;

		inline std::string GetFilePath() const override { return mFilePath; }
		void SetFilePath(const std::string& filePath) override;
	};
}