#include "ShaderProgram.h"
#include "Log.h"

ShaderProgram::ShaderProgram()
	:mId(0), mShaderProgramType(0)
{

}

void ShaderProgram::Unload()
{
	glDeleteProgram(mId);
}

void ShaderProgram::Compose(std::vector<Shader*> shaders)
{
	mId = glCreateProgram();
	mShaderProgramType = 0;
	for (int s = 0; s < static_cast<int>(shaders.size()); s++)
	{
		switch (shaders[s]->GetType()) {
		case ShaderType::VERTEX:
			mShaderProgramType += ShaderProgramType::VERTEX;
			break;
		case ShaderType::FRAGMENT:
			mShaderProgramType += ShaderProgramType::FRAGMENT;
			break;
		case ShaderType::TESSELLATION_CONTROL:
			mShaderProgramType += ShaderProgramType::TESSELLATION_CONTROL;
			break;
		case ShaderType::TESSELLATION_EVALUATION:
			mShaderProgramType += ShaderProgramType::TESSELLATION_EVALUATION;
			break;
		case ShaderType::GEOMETRY:
			mShaderProgramType += ShaderProgramType::GEOMETRY;
			break;
		}
		glAttachShader(mId, shaders[s]->GetID());
	}
	glLinkProgram(mId);
	Use();
	GLint success;
	glGetProgramiv(mId, GL_LINK_STATUS, &success);
	if (!success) {
		GLchar infoLog[512];
		glGetProgramInfoLog(mId, 512, NULL, infoLog);
		Log::Info("Shader program linking failed: " + std::string(infoLog));
	}
}

unsigned int ShaderProgram::GetID()
{
	return mId;
}

void ShaderProgram::Use()
{
	glUseProgram(mId);
}

void ShaderProgram::setFloat(const GLchar* name, GLfloat value)
{
	glUniform1f(glGetUniformLocation(mId, name), value);
}
void ShaderProgram::setInteger(const GLchar* name, GLint value)
{
	glUniform1i(glGetUniformLocation(mId, name), value);
}
void ShaderProgram::setVector2f(const GLchar* name, GLfloat x, GLfloat y)
{
	glUniform2f(glGetUniformLocation(mId, name), x, y);
}
void ShaderProgram::setVector2f(const GLchar* name, const Vector2D& value)
{
	glUniform2f(glGetUniformLocation(mId, name), value.x, value.y);
}
void ShaderProgram::setVector3f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z)
{
	glUniform3f(glGetUniformLocation(mId, name), x, y, z);
}
void ShaderProgram::setVector3f(const GLchar* name, const Vector3D& value)
{
	glUniform3f(glGetUniformLocation(mId, name), value.x, value.y, value.z);
}
void ShaderProgram::setVector4f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
	glUniform4f(glGetUniformLocation(mId, name), x, y, z, w);
}
void ShaderProgram::setVector4f(const GLchar* name, const Vector4D& value)
{
	glUniform4f(glGetUniformLocation(mId, name), value.x, value.y, value.z, value.w);
}
void ShaderProgram::setMatrix4(const GLchar* name, const Matrix4D& matrix)
{
	glUniformMatrix4fv(glGetUniformLocation(mId, name), 1, GL_TRUE, matrix.GetAsFloatPtr());
}
void ShaderProgram::setMatrix4Row(const GLchar* name, const Matrix4DRow& matrix)
{
	glUniformMatrix4fv(glGetUniformLocation(mId, name), 1, GL_TRUE, matrix.GetAsFloatPtr());
}

