#pragma once

#include <cstdint>
#include <string>

namespace Zephyrus::Assets
{
	class ITextureBase
	{
	public:
		virtual ~ITextureBase() = default; 
		virtual int GetWidth() const = 0;
		virtual int GetHeight() const = 0;
		virtual void Bind(unsigned int slot = 0) const = 0;
		virtual void Unbind(unsigned int slot = 0) const = 0;
		virtual uint64_t GetHandle() const = 0;
	};

}