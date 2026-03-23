#pragma once

#include "../Interface/IShader.h"

namespace Zephyrus::Render {
	/**
	 * @class Shader
	 * @brief Classe representant un shader OpenGL.
	 *
	 * Permet de charger le code source d'un shader, de stocker son identifiant et son type.
	 */
	class ShaderOpenGL : public IShader
	{
	protected:
		std::string mCode;
		unsigned int mId{ 0 };
		ShaderType mType{ ShaderType::VERTEX };
		std::string mShaderId;
	public:
		ShaderOpenGL() = default;
		ShaderOpenGL(int pId, const std::string& pCode, ShaderType pShaderType);
		~ShaderOpenGL() override;

		void Load(const std::string& pFileName, const std::string& id, ShaderType pShaderType) override;

		void Unload() override;

		inline int GetID() const override { return mId; }
		inline std::string GetShaderID() const override { return mShaderId; }

		std::string& GetCode() override;

		inline ShaderType GetType() const override { return mType; }
	};
}
