#pragma once
#include <optional>
#include "Vector3D.h"
#include "rapidjson/document.h"
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include <stack>
#include <fstream>
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

	class JsonWriter
	{
	private:
		rapidjson::Document mDocument;
		rapidjson::Document::AllocatorType* mAllocator;
		rapidjson::Value* mCurrentValue;
		std::stack<rapidjson::Value*> mParentStack;
		rapidjson::Value mTempArray { rapidjson::kArrayType };
	public:
		JsonWriter();
		~JsonWriter();
		void BeginObject(const char* pKey = nullptr);
		void EndObject();
		void BeginArray(const char* pKey);
		void EndArray();
		void WriteString(const char* pKey, const std::string& pValue);
		void WriteFloat(const char* pKey, float pValue);
		void WriteInt(const char* pKey, int pValue);
		void WriteBool(const char* pKey, bool pValue);
		void WriteVector3D(const char* pKey, const Vector3D& vec);
		bool SaveDocument(const std::string& filename);
		rapidjson::Document GetDoc()
		{
			return std::move(mDocument);
		}
	};
}
