#include "Shader.h"
#include "Log.h"
#include "glew.h"

namespace Zephyrus::Render {
	Shader::Shader()
		: mId(0), mCode(""), mType(ShaderType::VERTEX)
	{
	}

	Shader::Shader(int pId, const std::string& pCode, ShaderType pShaderType)
		: mId(pId), mCode(pCode), mType(pShaderType)
	{

	}

	Shader::~Shader()
	{
	}

	void Shader::Load(const std::string& pFileName, ShaderType pShaderType)
	{
		mType = pShaderType;
		std::ifstream myFile;

		myFile.open(pFileName);
		if (myFile.fail())
		{
			ZP_CORE_ERROR("Error - Failed to open" + pFileName);

		}
		std::string fileText = "";
		std::string line = "";
		while (getline(myFile, line))
		{
			fileText += line + '\n';
		}

		myFile.close();
		mCode = fileText;
		switch (mType)
		{
		case ShaderType::VERTEX:
			mId = glCreateShader(GL_VERTEX_SHADER);
			break;
		case ShaderType::FRAGMENT:
			mId = glCreateShader(GL_FRAGMENT_SHADER);
			break;
		case ShaderType::TESSELLATION_CONTROL:
			mId = glCreateShader(GL_TESS_CONTROL_SHADER);
			break;
		case ShaderType::TESSELLATION_EVALUATION:
			mId = glCreateShader(GL_TESS_EVALUATION_SHADER);
			break;
		case ShaderType::GEOMETRY:
			mId = glCreateShader(GL_GEOMETRY_SHADER);
			break;
		default:
		{
			ZP_CORE_ERROR("This shader type has not yet been implemented.");
			mId = glCreateShader(GL_VERTEX_SHADER);
			break;
		}
		break;
		}
		const char* source = mCode.c_str();
		glShaderSource(mId, 1, &source, NULL);
		glCompileShader(mId);
	}

	void Shader::Unload()
	{
		glDeleteShader(mId);
	}

	std::string& Shader::GetCode()
	{
		return mCode;
	}
}