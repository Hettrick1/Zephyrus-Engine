#pragma once
#include <optional>
#include "Vector3D.h"
#include "Vector2D.h"
#include "rapidjson/document.h"
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>
#include "ISerializer.h"
#include "IDeserializer.h"
#include <stack>
#include <fstream>
#include <string>
#include <vector>
#include "Log.h"

namespace Serialization::Json
{
	//std::optional<Vector3D> ReadVector3D(const rapidjson::Value& pObj, const char* pKey);
	//std::optional<Vector2D> ReadVector2D(const rapidjson::Value& pObj, const char* pKey);
	//std::optional<float> ReadFloat(const rapidjson::Value& pObj, const char* pKey);
	//std::optional<int> ReadInt(const rapidjson::Value& pObj, const char* pKey);
	//std::optional<bool> ReadBool(const rapidjson::Value& pObj, const char* pKey);
	//std::optional<std::string> ReadString(const rapidjson::Value& pObj, const char* pKey);

	//std::optional<std::vector<float>> ReadArrayFloat(const rapidjson::Value& pObj, const char* pKey);
	//std::optional<std::vector<int>> ReadArrayInt(const rapidjson::Value& pObj, const char* pKey);
	//std::optional<std::vector<bool>> ReadArrayBool(const rapidjson::Value& pObj, const char* pKey);
	//std::optional<std::vector<std::string>> ReadArrayString(const rapidjson::Value& pObj, const char* pKey);

	//const rapidjson::Value* ReadObject(const rapidjson::Value& pObj, const char* pKey);
	//std::optional<std::vector<const rapidjson::Value*>> ReadArrayObject(const rapidjson::Value& pObj, const char* pKey);

	class JsonReader : public IDeserializer
	{
	private:
	private:
		rapidjson::Document mDocument;
		const rapidjson::Value* mCurrentValue = nullptr;
		std::stack<const rapidjson::Value*> mContextStack;

		struct ArrayContext
		{
			const rapidjson::Value* arr = nullptr;
			rapidjson::Value::ConstValueIterator it;
			rapidjson::Value::ConstValueIterator end;
		};
		std::stack<ArrayContext> mArrayStack;

		const rapidjson::Value* GetMember(const rapidjson::Value* parent, const char* key);

	public:
		bool LoadDocument(const std::string& pFilepath) override;
		bool BeginObject(const char* pKey = nullptr) override;
		void EndObject() override;

		std::optional<std::string> ReadString(const char* pKey) override;
		std::optional<float> ReadFloat(const char* pKey) override;
		std::optional<int> ReadInt(const char* pKey) override;
		std::optional<bool> ReadBool(const char* pKey) override;
		std::optional<Vector3D> ReadVector3D(const char* pKey) override;
		std::optional<Vector2D> ReadVector2D(const char* pKey) override;

		template<typename T>
		std::optional<std::vector<T>> ReadArray(const char* pKey)
		{
			const auto* arr = GetMember(mCurrentValue, pKey);
			if (!arr || !arr->IsArray()) 
			{
				return std::nullopt;
			}

			std::vector<T> result;
			for (auto& val : arr->GetArray())
			{
				const auto* arr = GetMember(mCurrentValue, pKey);
				if (!arr || !arr->IsArray())
					return std::nullopt;

				std::vector<T> result;
				for (auto& val : arr->GetArray())
				{
					if constexpr (std::is_same_v<T, std::string>)
					{
						if (val.IsString()) result.push_back(val.GetString());
					}
					else if constexpr (std::is_same_v<T, float>)
					{
						if (val.IsNumber()) result.push_back(val.GetFloat());
					}
					else if constexpr (std::is_same_v<T, int>)
					{
						if (val.IsInt()) result.push_back(val.GetInt());
					}
					else if constexpr (std::is_same_v<T, bool>)
					{
						if (val.IsBool()) result.push_back(val.GetBool());
					}
					else
					{
						ZP_CORE_ERROR("Type not supported for ReadArray");
					}
				}
				return result;
			}
			return result;
		}

		std::optional<std::vector<std::string>> ReadArrayString(const char* pKey) override;
		std::optional<std::vector<float>> ReadArrayFloat(const char* pKey) override;
		std::optional<std::vector<int>> ReadArrayInt(const char* pKey) override;
		std::optional<std::vector<bool>> ReadArrayBool(const char* pKey) override;
		std::optional<std::vector<Vector3D>> ReadArrayVector3D(const char* pKey) override;
		std::optional<std::vector<Vector2D>> ReadArrayVector2D(const char* pKey) override;

		bool BeginObjectArray(const char* pKey) override;
		bool NextObjectElement() override;
		void EndObjectArray() override;
	};

	class JsonWriter : public ISerializer
	{
	private:
		rapidjson::Document mDocument;
		rapidjson::Document::AllocatorType* mAllocator;
		rapidjson::Value* mCurrentValue;
		std::stack<rapidjson::Value*> mParentStack;
	public:
		JsonWriter();
		void BeginObject(const char* pKey = nullptr) override;
		void EndObject() override;
		void BeginArray(const char* pKey) override;
		void EndArray() override;
		void WriteString(const char* pKey, const std::string& pValue) override;
		void WriteFloat(const char* pKey, float pValue) override;
		void WriteInt(const char* pKey, int pValue) override;
		void WriteBool(const char* pKey, bool pValue) override;
		void WriteVector3D(const char* pKey, const Vector3D& pVec) override;
		void WriteVector2D(const char* pKey, const Vector2D& pVec) override;
		void PushString(const std::string& pValue) override;
		void PushFloat(float pValue) override;
		void PushInt(int pValue) override;
		void PushBool(bool pValue) override;
		bool SaveDocument(const std::string& pFilepath) override;
		const rapidjson::Document& GetDoc()
		{
			return mDocument;
		}
	};
}
