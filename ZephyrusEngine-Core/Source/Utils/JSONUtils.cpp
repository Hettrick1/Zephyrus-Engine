#include "JSONUtils.h"
#include <string>
#include "Log.h"

namespace Serialization::Json {

    std::optional<Vector3D> ReadVector3D(const rapidjson::Value& pObj, const char* pKey)
    {
        if (!pObj.HasMember(pKey) || !pObj[pKey].IsArray())
        {
            return std::nullopt;
        }
        const auto& array = pObj[pKey].GetArray();
        if (array.Size() != 3)
        {
            ZP_CORE_ERROR(std::string(pKey) + " must be an array of 3 floats!");
            return std::nullopt;
        }
        return Vector3D(array[0].GetFloat(), array[1].GetFloat(), array[2].GetFloat());
    }
    std::optional<Vector2D> ReadVector2D(const rapidjson::Value& pObj, const char* pKey)
    {
        if (!pObj.HasMember(pKey) || !pObj[pKey].IsArray())
        {
            return std::nullopt;
        }
        const auto& array = pObj[pKey].GetArray();
        if (array.Size() != 3)
        {
            ZP_CORE_ERROR(std::string(pKey) + " must be an array of 2 floats!");
            return std::nullopt;
        }
        return Vector2D(array[0].GetFloat(), array[1].GetFloat());
    }
    std::optional<float> ReadFloat(const rapidjson::Value& pObj, const char* pKey)
    {
        if (!pObj.HasMember(pKey) || !pObj[pKey].IsNumber())
            return std::nullopt;
        return pObj[pKey].GetFloat();
    }
    std::optional<int> ReadInt(const rapidjson::Value& pObj, const char* pKey)
    {
        if (!pObj.HasMember(pKey) || !pObj[pKey].IsInt())
            return std::nullopt;
        return pObj[pKey].GetInt();
    }
    std::optional<bool> ReadBool(const rapidjson::Value& pObj, const char* pKey)
    {
        if (!pObj.HasMember(pKey) || !pObj[pKey].IsBool())
            return std::nullopt;
        return pObj[pKey].GetBool();
    }
    std::optional<std::string> ReadString(const rapidjson::Value& pObj, const char* pKey)
    {
        if (!pObj.HasMember(pKey) || !pObj[pKey].IsString())
            return std::nullopt;
        return pObj[pKey].GetString();
    }
    std::optional<std::vector<float>> ReadArrayFloat(const rapidjson::Value& pObj, const char* pKey)
    {
        if (!pObj.HasMember(pKey) || !pObj[pKey].IsArray())
        {
            return std::nullopt;
        }
        std::vector<float> result;
        for (auto& v : pObj[pKey].GetArray())
        {
            if (v.IsNumber()) {
                result.push_back(v.GetFloat());
            }
        }
       
        return result;
    }
    std::optional<std::vector<int>> ReadArrayInt(const rapidjson::Value& pObj, const char* pKey)
    {
        if (!pObj.HasMember(pKey) || !pObj[pKey].IsArray())
        {
            return std::nullopt;
        }
        std::vector<int> result;
        for (auto& v : pObj[pKey].GetArray())
        {
            if (v.IsInt()) {
                result.push_back(v.GetInt());
            }
        }

        return result;
    }
    std::optional<std::vector<bool>> ReadArrayBool(const rapidjson::Value& pObj, const char* pKey)
    {
        if (!pObj.HasMember(pKey) || !pObj[pKey].IsArray())
        {
            return std::nullopt;
        }
        std::vector<bool> result;
        for (auto& v : pObj[pKey].GetArray())
        {
            if (v.IsBool()) {
                result.push_back(v.GetBool());
            }
        }

        return result;
    }
    std::optional<std::vector<std::string>> ReadArrayString(const rapidjson::Value& pObj, const char* pKey)
    {
        if (!pObj.HasMember(pKey) || !pObj[pKey].IsArray())
        {
            return std::nullopt;
        }
        std::vector<std::string> result;
        for (auto& v : pObj[pKey].GetArray())
        {
            if (v.IsString()) {
                result.push_back(v.GetString());
            }
        }

        return result;
    }
    const rapidjson::Value* ReadObject(const rapidjson::Value& pObj, const char* pKey)
    {
        if (pObj.HasMember(pKey) && pObj[pKey].IsObject()) {
            return &pObj[pKey];
        }
        else
        {
            return nullptr;
        }
    }
    std::optional<std::vector<const rapidjson::Value*>> ReadArrayObject(const rapidjson::Value& pObj, const char* pKey)
    {
        if (!pObj.HasMember(pKey) || !pObj[pKey].IsArray()) {
            return std::nullopt;
        }
        std::vector<const rapidjson::Value*> result;
        for (auto& v : pObj[pKey].GetArray())
        {
            if (v.IsObject()) {
                result.push_back(&v);
            }
        }
        return result;
    }

    JsonWriter::JsonWriter()
    {
        mDocument.SetObject();
        mAllocator = &mDocument.GetAllocator();
        mCurrentValue = &mDocument;
    }
    void JsonWriter::BeginObject(const char* pKey)
    {
        rapidjson::Value obj(rapidjson::kObjectType);

        if (pKey)
        {
            mCurrentValue->AddMember(rapidjson::Value(pKey, *mAllocator).Move(), std::move(obj), *mAllocator);
            mParentStack.push(mCurrentValue);
            mCurrentValue = &(*mCurrentValue)[pKey];
        }
        else
        {
            mCurrentValue->PushBack(std::move(obj), *mAllocator);
            mParentStack.push(mCurrentValue);
            mCurrentValue = &(*mCurrentValue)[mCurrentValue->Size() - 1];
        }
    }
    void JsonWriter::EndObject()
    {
        mCurrentValue = mParentStack.top();
        mParentStack.pop();
    }
    void JsonWriter::BeginArray(const char* pKey)
    {
        rapidjson::Value arr(rapidjson::kArrayType);

        mCurrentValue->AddMember(rapidjson::Value(pKey, *mAllocator).Move(), arr, *mAllocator);
        mParentStack.push(mCurrentValue);
        mCurrentValue = &(*mCurrentValue)[pKey];
    }
    void JsonWriter::EndArray()
    {
        mCurrentValue = mParentStack.top();
        mParentStack.pop();
    }
    void JsonWriter::WriteString(const char* pKey, const std::string& pValue)
    {
        mCurrentValue->AddMember(rapidjson::Value(pKey, *mAllocator).Move(), rapidjson::Value(pValue.c_str(), *mAllocator).Move(), *mAllocator);
    }
    void JsonWriter::WriteFloat(const char* pKey, float pValue)
    {
        mCurrentValue->AddMember(rapidjson::Value(pKey, *mAllocator).Move(), rapidjson::Value(pValue).Move(), *mAllocator);
    }
    void JsonWriter::WriteInt(const char* pKey, int pValue)
    {
        mCurrentValue->AddMember(rapidjson::Value(pKey, *mAllocator).Move(), rapidjson::Value(pValue).Move(), *mAllocator);
    }
    void JsonWriter::WriteBool(const char* pKey, bool pValue)
    {
        mCurrentValue->AddMember(rapidjson::Value(pKey, *mAllocator).Move(), rapidjson::Value(pValue).Move(), *mAllocator);
    }
    void JsonWriter::WriteVector3D(const char* pKey, const Vector3D& pVec)
    {
        rapidjson::Value arr(rapidjson::kArrayType);
        arr.PushBack(pVec.x, *mAllocator);
        arr.PushBack(pVec.y, *mAllocator);
        arr.PushBack(pVec.z, *mAllocator);
        mCurrentValue->AddMember(rapidjson::Value(pKey, *mAllocator).Move(), arr, *mAllocator);
    }
    void JsonWriter::PushString(const std::string& pValue)
    {
        if (mCurrentValue->IsArray())
            mCurrentValue->PushBack(rapidjson::Value(pValue.c_str(), *mAllocator).Move(), *mAllocator);
    }
    void JsonWriter::PushFloat(float pValue)
    {
        if (mCurrentValue->IsArray())
        {
            mCurrentValue->PushBack(rapidjson::Value(pValue).Move(), *mAllocator);
        }
    }
    void JsonWriter::PushInt(int pValue)
    {
        if (mCurrentValue->IsArray())
        {
            mCurrentValue->PushBack(rapidjson::Value(pValue).Move(), *mAllocator);
        }
    }
    void JsonWriter::PushBool(bool pValue)
    {
        if (mCurrentValue->IsArray())
        {
            mCurrentValue->PushBack(rapidjson::Value(pValue).Move(), *mAllocator);
        }
    }
    bool JsonWriter::SaveDocument(const std::string& pFilepath)
    {
        rapidjson::StringBuffer buffer;

        // TODO replace prettyWriter by Writter it's jus for debugging purposes

        rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
        writer.SetFormatOptions(rapidjson::kFormatSingleLineArray);
        writer.SetMaxDecimalPlaces(3);
        mDocument.Accept(writer);

        std::ofstream file(pFilepath);
        if (!file.is_open()) 
        {
            return false;
        }
        file << buffer.GetString();

        return true;
    }
}
