#pragma once

#include "../Interface/IShader.h"

namespace Zephyrus::Render {
	/**
	 * @class Shader
	 * @brief Classe représentant un shader OpenGL.
	 *
	 * Permet de charger le code source d'un shader, de stocker son identifiant et son type.
	 */
	class Shader : public IShader
	{
	protected:
		std::string mCode{ "" };
		unsigned int mId{ 0 };
		ShaderType mType{ ShaderType::VERTEX };
	public:
		Shader() = default;
		Shader(int pId, const std::string& pCode, ShaderType pShaderType);
		~Shader();

		void Load(const std::string& pFileName, ShaderType pShaderType) override;

		void Unload() override;

		inline int GetID() const override { return mId; }

		std::string& GetCode() override;

		inline ShaderType GetType() const override { return mType; }
	};
}