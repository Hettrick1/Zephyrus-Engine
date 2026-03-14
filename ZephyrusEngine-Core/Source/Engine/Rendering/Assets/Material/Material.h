#pragma once

#include "Interface/ITextureBase.h"
#include "IMaterial.h"
#include <unordered_map>
#include <utility>

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

		std::vector<std::pair<std::string, Assets::ITextureBase*>> mTextures;
		std::vector<std::pair<std::string, int>> mIntProperties;
		std::vector<std::pair<std::string, float>> mfloatProperties;
		std::vector<std::pair<std::string, Vector2D>> mVec2Properties;
		std::vector<std::pair<std::string, Vector3D>> mVec3Properties;
		std::vector<std::pair<std::string, Vector4D>> mVec4Properties;
		std::vector<std::pair<std::string, Matrix4DRow>> mMat4Properties;

		std::string mFilePath{ " " };

	public:

		Material();
		~Material() override;

		void SetVertexShader(Render::IShader* s) override;
		void SetFragmentShader(Render::IShader* s) override;
		void SetTessControlShader(Render::IShader* s) override;
		void SetTessEvalShader(Render::IShader* s) override;
		void SetGeometryShader(Render::IShader* s) override;

		void RebuildShaderProgram() override;

		inline Render::IShaderProgram* GetShaderProgram() override { return mShaderProgram; }

		void SetTexture(const std::string& uniformName, Assets::ITextureBase* texture) override;

		template<typename T>
		void UpdateOrAddProperty(std::vector<std::pair<std::string, T>>& container, const std::string& name, const T& value)
		{
			auto it = std::find_if(container.begin(), container.end(),
			[&name](const auto& pair) { return pair.first == name; });

			if (it != container.end()) {
				it->second = value;
			} else {
				RemoveProperty(name); 
				container.emplace_back(name, value);
			}
		}
		
		void SetProperty(const std::string& uniformName, float value) override;
		void SetProperty(const std::string& uniformName, int value) override;
		void SetProperty(const std::string& uniformName, const Vector2D& value) override;
		void SetProperty(const std::string& uniformName, const Vector3D& value) override;
		void SetProperty(const std::string& uniformName, const Vector4D& value) override;
		void SetProperty(const std::string& uniformName, const Matrix4DRow& value) override;
		void RemoveProperty(const std::string& uniformName) override;

		inline std::vector<std::pair<std::string, float>> GetFloatProperties() override { return mfloatProperties; }
		inline std::vector<std::pair<std::string, int>> GetIntProperties() override { return mIntProperties; }
		inline std::vector<std::pair<std::string, Vector2D>> GetVec2Properties() override { return mVec2Properties; }
		inline std::vector<std::pair<std::string, Vector3D>> GetVec3Properties() override { return mVec3Properties; }
		inline std::vector<std::pair<std::string, Vector4D>> GetVec4Properties() override { return mVec4Properties; }
		inline std::vector<std::pair<std::string, Matrix4DRow>> GetMatrix4Properties() override { return mMat4Properties; }
		inline std::vector<std::pair<std::string, Assets::ITextureBase*>> GetTextureProperties() override { return mTextures; }

		void Use(const Matrix4DRow* world = nullptr, const Matrix4DRow* viewproj = nullptr) override;

		void Serialize(Serialization::ISerializer& writer) const override;
		void Deserialize(Serialization::IDeserializer& reader) override;

		std::vector<PropertyDescriptor> GetProperties() override;
		
		inline std::string GetFilePath() const override { return mFilePath; }
		void SetFilePath(const std::string& filePath) override;
	};
}