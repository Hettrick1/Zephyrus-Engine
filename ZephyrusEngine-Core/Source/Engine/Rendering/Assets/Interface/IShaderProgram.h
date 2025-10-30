#pragma once

#include "Matrix4DRow.h"
#include "Matrix4D.h"
#include "Vector2D.h"
#include "Vector3D.h"
#include "Vector4D.h"

#include "Interface/IShader.h"

#include <vector>

#include <stdint.h>

namespace Zephyrus::ShaderProgramType {
	enum Type {
		VERTEX = 1 << 0,
		FRAGMENT = 1 << 1,
		TESSELLATION_CONTROL = 1 << 2,
		TESSELLATION_EVALUATION = 1 << 3,
		GEOMETRY = 1 << 4
	};

	inline uint8_t operator + (uint8_t& a, Type& b) {
		return a | static_cast<uint8_t>(b);
	}

	inline uint8_t operator - (uint8_t& a, Type& b) {
		return a ^ static_cast<uint8_t>(b);
	}
	inline void operator += (uint8_t& a, Type b) {
		a |= static_cast<uint8_t>(b);
	}

	inline void operator -= (uint8_t& a, Type b) {
		a ^= static_cast<uint8_t>(b);
	}
}

namespace Zephyrus::Render 
{
	class IShaderProgram
	{
	public:
		virtual ~IShaderProgram() = default;
		// Unloads the shader program from GPU memory
		virtual void Unload() = 0;
		
		// Composes the shader program from a list of shaders
		virtual void Compose(std::vector<IShader*>& pShaders) = 0;
		 
		virtual inline unsigned int GetID() const = 0;
		virtual inline uint8_t GetType() const = 0;
 
		virtual void Use() = 0;
		
		// Sets a float uniform variable in the shader
		virtual void setFloat(const char* name, float value) = 0;
		// Sets an integer uniform variable in the shader
		virtual void setInteger(const char* name, int value) = 0;
		// Sets a vec2 uniform variable in the shader (from two floats)
		virtual void setVector2f(const char* name, float x, float y) = 0;
		// Sets a vec2 uniform variable in the shader (from a Vector2D)
		virtual void setVector2f(const char* name, const Vector2D& value) = 0;
		// Sets a vec3 uniform variable in the shader (from three floats)
		virtual void setVector3f(const char* name, float x, float y, float z) = 0;
		// Sets a vec3 uniform variable in the shader (from a Vector3D)
		virtual void setVector3f(const char* name, const Vector3D& value) = 0;
		// Sets a vec4 uniform variable in the shader (from four floats)
		virtual void setVector4f(const char* name, float x, float y, float z, float w) = 0;
		// Sets a vec4 uniform variable in the shader (from a Vector4D)
		virtual void setVector4f(const char* name, const Vector4D& value) = 0;
		// Sets a mat4 uniform variable in the shader (column-major)
		virtual void setMatrix4(const char* name, const Matrix4D& matrix) = 0;
		// Sets a mat4 uniform variable in the shader (row-major)
		virtual void setMatrix4Row(const char* name, const Matrix4DRow& matrix) = 0;
	}; 
}