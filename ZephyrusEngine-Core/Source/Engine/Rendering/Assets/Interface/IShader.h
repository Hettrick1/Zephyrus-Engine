#pragma once

#include <string>
#include <fstream>


namespace Zephyrus::Render {
	enum class ShaderType {
		VERTEX,
		FRAGMENT,
		TESSELLATION_CONTROL,
		TESSELLATION_EVALUATION,
		GEOMETRY
	};
	class IShader
	{
	public:
		virtual ~IShader() = default;

		virtual void Load(const std::string& pFileName, ShaderType pShaderType) = 0;
		virtual void Unload() = 0;
		virtual int GetID() const = 0;
		virtual std::string& GetCode() = 0;
		virtual ShaderType GetType() const = 0;
	};
}