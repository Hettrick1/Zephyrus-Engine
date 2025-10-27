#pragma once

#include "glew.h"

#include "Interface/IShader.h"

#include "Matrix4DRow.h"
#include "Matrix4D.h"
#include "Vector2D.h"
#include "Vector3D.h"
#include "Vector4D.h"
#include "ShaderProgramType.h"

#include <vector>

/**
 * @class ShaderProgram
 * @brief Manages an OpenGL shader program, allowing composition, usage, and uniform variable setting.
 */
namespace Zephyrus::Render {
	class ShaderProgram
	{
	private:
		unsigned int mId;
		uint8_t mShaderProgramType;

	public:
		ShaderProgram();
		~ShaderProgram() = default;

		// Unloads the shader program from GPU memory
		void Unload();

		// Composes the shader program from a list of shaders
		void Compose(std::vector<IShader*>& pShaders);

		inline unsigned int GetID() const { return mId; }
		inline uint8_t GetType() const { return mShaderProgramType; }
		void Use();

		// Sets a float uniform variable in the shader
		void setFloat(const GLchar* name, GLfloat value);
		// Sets an integer uniform variable in the shader
		void setInteger(const GLchar* name, GLint value);
		// Sets a vec2 uniform variable in the shader (from two floats)
		void setVector2f(const GLchar* name, GLfloat x, GLfloat y);
		// Sets a vec2 uniform variable in the shader (from a Vector2D)
		void setVector2f(const GLchar* name, const Vector2D& value);
		// Sets a vec3 uniform variable in the shader (from three floats)
		void setVector3f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z);
		// Sets a vec3 uniform variable in the shader (from a Vector3D)
		void setVector3f(const GLchar* name, const Vector3D& value);
		// Sets a vec4 uniform variable in the shader (from four floats)
		void setVector4f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
		// Sets a vec4 uniform variable in the shader (from a Vector4D)
		void setVector4f(const GLchar* name, const Vector4D& value);
		// Sets a mat4 uniform variable in the shader (column-major)
		void setMatrix4(const GLchar* name, const Matrix4D& matrix);
		// Sets a mat4 uniform variable in the shader (row-major)
		void setMatrix4Row(const GLchar* name, const Matrix4DRow& matrix);
	};
}