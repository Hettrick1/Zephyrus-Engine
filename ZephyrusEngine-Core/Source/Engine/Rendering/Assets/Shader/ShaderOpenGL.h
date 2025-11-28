#pragma once

#include "../Interface/IShader.h"
#include "FileSystem/AssetData.h"

namespace Zephyrus::Render {
	/**
	 * @class Shader
	 * @brief Classe representant un shader OpenGL.
	 *
	 * Permet de charger le code source d'un shader, de stocker son identifiant et son type.
	 */
	class ShaderOpenGL : public IShader, public Assets::AssetData
	{
	protected:
		std::string mCode;
		unsigned int mId{ 0 };
		ShaderType mType{ ShaderType::VERTEX };
		std::string mFilePath;
	public:
		ShaderOpenGL(int pId, const std::string& pCode, ShaderType pShaderType);
		~ShaderOpenGL() override;

		void Load(const std::string& pFileName, ShaderType pShaderType) override;

		void Unload() override;

		inline int GetID() const override { return mId; }
		inline std::string GetFilePath() const override { return mFilePath; }

		std::string& GetCode() override;

		inline ShaderType GetType() const override { return mType; }
	};
}
