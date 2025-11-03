#pragma once

#include <string>
#include <optional>
#include <vector>
#include "Vector2D.h"
#include "Vector3D.h"
#include "Vector4D.h"
#include "Matrix4DRow.h"

namespace Serialization
{
	class IDeserializer
	{
	public:
		virtual ~IDeserializer() = default;

		virtual bool LoadDocument(const std::string& pFilepath) = 0;
		virtual bool BeginObject(const char* pKey = nullptr) = 0;
		virtual void EndObject() = 0;

		virtual std::string GetCurrentKey() = 0;

		virtual std::optional<std::string> ReadString(const char* pKey = nullptr) = 0;
		virtual std::optional<float> ReadFloat(const char* pKey = nullptr) = 0;
		virtual std::optional<int> ReadInt(const char* pKey = nullptr) = 0;
		virtual std::optional<bool> ReadBool(const char* pKey = nullptr) = 0;
		virtual std::optional<Vector4D> ReadVector4D(const char* pKey = nullptr) = 0;
		virtual std::optional<Vector3D> ReadVector3D(const char* pKey = nullptr) = 0;
		virtual std::optional<Vector2D> ReadVector2D(const char* pKey = nullptr) = 0;
		virtual std::optional<Matrix4DRow> ReadMatrix4DRow(const char* pKey = nullptr) = 0;

		virtual std::optional<std::vector<std::string>> ReadArrayString(const char* pKey = nullptr) = 0;
		virtual std::optional<std::vector<float>> ReadArrayFloat(const char* pKey = nullptr) = 0;
		virtual std::optional<std::vector<int>> ReadArrayInt(const char* pKey = nullptr) = 0;
		virtual std::optional<std::vector<bool>> ReadArrayBool(const char* pKey = nullptr) = 0;
		virtual std::optional<std::vector<Vector4D>> ReadArrayVector4D(const char* pKey = nullptr) = 0;
		virtual std::optional<std::vector<Vector3D>> ReadArrayVector3D(const char* pKey = nullptr) = 0;
		virtual std::optional<std::vector<Vector2D>> ReadArrayVector2D(const char* pKey = nullptr) = 0;

		virtual bool BeginObjectArray(const char* pKey) = 0;
		virtual bool NextObjectElement() = 0;
		virtual void EndObjectArray() = 0;
	};
}