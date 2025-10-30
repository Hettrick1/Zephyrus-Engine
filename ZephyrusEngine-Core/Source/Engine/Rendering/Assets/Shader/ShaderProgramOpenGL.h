#pragma once

#include "glew.h"

#include "Interface/IShaderProgram.h"

/**
 * @class ShaderProgram
 * @brief Manages an OpenGL shader program, allowing composition, usage, and uniform variable setting.
 */
namespace Zephyrus::Render {
	class ShaderProgram : public IShaderProgram
	{
	private:
		unsigned int mId{ 0 };
		uint8_t mShaderProgramType{ 0 };

	public:
		ShaderProgram() = default;
		// Unloads the shader program from GPU memory
		void Unload() override;

		// Composes the shader program from a list of shaders
		void Compose(std::vector<IShader*>& pShaders) override;

		inline unsigned int GetID() const override { return mId; }
		inline uint8_t GetType() const override { return mShaderProgramType; }
		void Use();

		// Sets a float uniform variable in the shader
		void setFloat(const GLchar* name, GLfloat value) override;
		// Sets an integer uniform variable in the shader
		void setInteger(const GLchar* name, GLint value) override;
		// Sets a vec2 uniform variable in the shader (from two floats)
		void setVector2f(const GLchar* name, GLfloat x, GLfloat y) override;
		// Sets a vec2 uniform variable in the shader (from a Vector2D)
		void setVector2f(const GLchar* name, const Vector2D& value) override;
		// Sets a vec3 uniform variable in the shader (from three floats)
		void setVector3f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z) override;
		// Sets a vec3 uniform variable in the shader (from a Vector3D)
		void setVector3f(const GLchar* name, const Vector3D& value) override;
		// Sets a vec4 uniform variable in the shader (from four floats)
		void setVector4f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w) override;
		// Sets a vec4 uniform variable in the shader (from a Vector4D)
		void setVector4f(const GLchar* name, const Vector4D& value) override;
		// Sets a mat4 uniform variable in the shader (column-major)
		void setMatrix4(const GLchar* name, const Matrix4D& matrix) override;
		// Sets a mat4 uniform variable in the shader (row-major)
		void setMatrix4Row(const GLchar* name, const Matrix4DRow& matrix) override;
	};
}