#pragma once
#include <optional>
#include "Vector3D.h"
#include "rapidjson/document.h"
#include <string>
#include <vector>

namespace Serialization::Json
{
	std::optional<Vector3D> ReadVector3D(const rapidjson::Value& pObj, const char* pKey);
	std::optional<float> ReadFloat(const rapidjson::Value& pObj, const char* pKey);
	std::optional<int> ReadInt(const rapidjson::Value& pObj, const char* pKey);
	std::optional<bool> ReadBool(const rapidjson::Value& pObj, const char* pKey);
	std::optional<std::string> ReadString(const rapidjson::Value& pObj, const char* pKey);

	std::optional<std::vector<float>> ReadArrayFloat(const rapidjson::Value& pObj, const char* pKey);
	std::optional<std::vector<int>> ReadArrayInt(const rapidjson::Value& pObj, const char* pKey);
	std::optional<std::vector<bool>> ReadArrayBool(const rapidjson::Value& pObj, const char* pKey);
	std::optional<std::vector<std::string>> ReadArrayString(const rapidjson::Value& pObj, const char* pKey);

	const rapidjson::Value* ReadObject(const rapidjson::Value& pObj, const char* pKey);
	std::optional<std::vector<const rapidjson::Value*>> ReadArrayObject(const rapidjson::Value& pObj, const char* pKey);

}
