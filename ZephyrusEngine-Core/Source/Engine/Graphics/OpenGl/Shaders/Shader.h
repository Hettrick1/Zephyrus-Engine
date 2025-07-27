#pragma once

#include <string>
#include <fstream>

enum class ShaderType {
	VERTEX,
	FRAGMENT,
	TESSELLATION_CONTROL,
	TESSELLATION_EVALUATION,
	GEOMETRY
};

/**
 * @class Shader
 * @brief Classe représentant un shader OpenGL.
 *
 * Permet de charger le code source d'un shader, de stocker son identifiant et son type.
 */
class Shader
{
protected:
	std::string mCode;
	unsigned int mId;
	ShaderType mType;
public:
	static const std::string SHADER_PATH;
	Shader();
	Shader(int pId, const std::string& pCode, ShaderType pShaderType);
	~Shader();

	void Load(const std::string& pFileName, ShaderType pShaderType);

	void Unload();

	int GetID() const { return mId; }

	std::string& GetCode();

	inline ShaderType GetType() const { return mType; }   
};