#pragma once

#include <stdint.h>

namespace ShaderProgramType {
	enum Type {
		VERTEX					= 1 << 0,
		FRAGMENT				= 1 << 1,
		TESSELLATION_CONTROL	= 1 << 2,
		TESSELLATION_EVALUATION = 1 << 3,
		GEOMETRY				= 1 << 4
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