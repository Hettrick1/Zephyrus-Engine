#include "JSONUtils.h"
#include <string>
#include "Log.h"

namespace Serialization::Json {

    const rapidjson::Value* JsonReader::GetMember(const rapidjson::Value* parent, const char* key)
    {
        if (!parent || !parent->IsObject()) return nullptr;
        auto it = parent->FindMember(key);
        if (it == parent->MemberEnd()) return nullptr;
        return &it->value;
    }

    bool JsonReader::LoadDocument(const std::string& pFilepath)
    {

        std::ifstream file(pFilepath);

        if (!file.is_open())
        {
            ZP_CORE_ERROR("Impossible to open the prefab : " + pFilepath);
            return false;
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string jsonContent = buffer.str();

        mDocument.Parse(jsonContent.c_str());
        if (mDocument.HasParseError()) {
            ZP_CORE_ERROR(pFilepath + " Parsing JSON failed !");
            return false;
        }
        mCurrentValue = &mDocument;
        return true;
    }
    bool JsonReader::BeginObject(const char* pKey)
    {
        const rapidjson::Value* obj = (pKey) ? GetMember(mCurrentValue, pKey) : mCurrentValue;
        if (!obj || !obj->IsObject()) return false;

        mContextStack.push(mCurrentValue);
        mCurrentValue = obj;
        mObjectIterator = mCurrentValue->MemberBegin();
        return true;
    }
    void JsonReader::EndObject()
    {
        if (!mContextStack.empty())
        {
            mCurrentValue = mContextStack.top();
            mContextStack.pop();
        }
    }
    std::string JsonReader::GetCurrentKey()
    {
        return mCurrentKey;
    }
    std::optional<std::string> JsonReader::ReadString(const char* pKey)
    {
        const auto* v = pKey ? GetMember(mCurrentValue, pKey) : mCurrentValue;
        if (!v || !v->IsString()) 
        {
            return std::nullopt;
        }
        return v->GetString();
    }
    std::optional<float> JsonReader::ReadFloat(const char* pKey)
    {
        const auto* v = pKey ? GetMember(mCurrentValue, pKey) : mCurrentValue;
        if (!v || !v->IsFloat())
        {
            return std::nullopt;
        }
        return v->GetFloat();
    }
    std::optional<int> JsonReader::ReadInt(const char* pKey)
    {
        const auto* v = pKey ? GetMember(mCurrentValue, pKey) : mCurrentValue;
        if (!v || !v->IsInt())
        {
            return std::nullopt;
        }
        return v->GetInt();
    }
    std::optional<bool> JsonReader::ReadBool(const char* pKey)
    {
        const auto* v = pKey ? GetMember(mCurrentValue, pKey) : mCurrentValue;
        if (!v || !v->IsBool())
        {
            return std::nullopt;
        }
        return v->GetBool();
    }
    std::optional<Vector4D> JsonReader::ReadVector4D(const char* pKey)
    {
        const auto* v = pKey ? GetMember(mCurrentValue, pKey) : mCurrentValue;
        if (!v || !v->IsArray() || v->Size() != 4)
        {
            return std::nullopt;
        }
        return Vector4D((*v)[0].GetFloat(), (*v)[1].GetFloat(), (*v)[2].GetFloat(), (*v)[3].GetFloat());
    }
    std::optional<Vector3D> JsonReader::ReadVector3D(const char* pKey)
    {
        const auto* v = pKey ? GetMember(mCurrentValue, pKey) : mCurrentValue;
        if (!v || !v->IsArray() || v->Size() != 3) 
        {
            return std::nullopt;
        }
        return Vector3D((*v)[0].GetFloat(), (*v)[1].GetFloat(), (*v)[2].GetFloat());
    }
    std::optional<Vector2D> JsonReader::ReadVector2D(const char* pKey)
    {
        const auto* v = pKey ? GetMember(mCurrentValue, pKey) : mCurrentValue;
        if (!v || !v->IsArray() || v->Size() != 2) 
        {
            return std::nullopt;
        }
        return Vector2D((*v)[0].GetFloat(), (*v)[1].GetFloat());
    }

    std::optional<Matrix4DRow> JsonReader::ReadMatrix4DRow(const char* pKey)
    {
        return std::nullopt;
    }

    std::optional<std::vector<std::string>> JsonReader::ReadArrayString(const char* pKey)
    {
        return ReadArray<std::string>(pKey);
    }
    std::optional<std::vector<float>> JsonReader::ReadArrayFloat(const char* pKey)
    {
        return ReadArray<float>(pKey);
    }
    std::optional<std::vector<int>> JsonReader::ReadArrayInt(const char* pKey)
    {
        return ReadArray<int>(pKey);
    }
    std::optional<std::vector<bool>> JsonReader::ReadArrayBool(const char* pKey)
    {
        return ReadArray<bool>(pKey);
    }
    std::optional<std::vector<Vector4D>> JsonReader::ReadArrayVector4D(const char* pKey)
    {
        const auto* arr = pKey ? GetMember(mCurrentValue, pKey) : mCurrentValue;
        if (!arr || !arr->IsArray())
        {
            return std::nullopt;
        }

        std::vector<Vector4D> result;
        for (auto& val : arr->GetArray())
        {
            if (val.IsArray() && val.Size() == 4)
            {
                result.emplace_back(val[0].GetFloat(), val[1].GetFloat(), val[2].GetFloat(), val[3].GetFloat());
            }
        }
        return result;
    }
    std::optional<std::vector<Vector3D>> JsonReader::ReadArrayVector3D(const char* pKey)
    {
        const auto* arr = pKey ? GetMember(mCurrentValue, pKey) : mCurrentValue;
        if (!arr || !arr->IsArray()) 
        {
            return std::nullopt;
        }

        std::vector<Vector3D> result;
        for (auto& val : arr->GetArray())
        {
            if (val.IsArray() && val.Size() == 3)
            {
                result.emplace_back(val[0].GetFloat(), val[1].GetFloat(), val[2].GetFloat());
            }
        }
        return result;
    }
    std::optional<std::vector<Vector2D>> JsonReader::ReadArrayVector2D(const char* pKey)
    {
        const auto* arr = pKey ? GetMember(mCurrentValue, pKey) : mCurrentValue;
        if (!arr || !arr->IsArray()) 
        {
            return std::nullopt;
        }

        std::vector<Vector2D> result;
        for (auto& val : arr->GetArray())
        {
            if (val.IsArray() && val.Size() == 2)
            {
                result.emplace_back(val[0].GetFloat(), val[1].GetFloat());
            }
        }
        return result;
    }
    bool JsonReader::BeginObjectArray(const char* pKey)
    {
        const rapidjson::Value* arr = GetMember(mCurrentValue, pKey);
        if (!arr || !arr->IsArray())
        {
            return false;
        }

        mArrayStack.push({ arr, arr->Begin(), arr->End() });
        return true;
    }
    bool JsonReader::NextObjectElement()
    {
        if (mArrayStack.empty())
            return false;

        auto& ctx = mArrayStack.top();
        if (ctx.it == ctx.end)
            return false;

        if (!ctx.it->IsObject())
        {
            ++ctx.it;
            return false;
        }

        mContextStack.push(mCurrentValue);
        mCurrentValue = &(*ctx.it++);
        return true;
    }
    void JsonReader::EndObjectArray()
    {
        if (mArrayStack.empty())
            return;

        if (!mContextStack.empty())
        {
            mCurrentValue = mContextStack.top();
            mContextStack.pop();
        }

        mArrayStack.pop();
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
    void JsonWriter::WriteVector4D(const char* pKey, const Vector4D& pVec)
    {
        rapidjson::Value arr(rapidjson::kArrayType);
        arr.PushBack(pVec.x, *mAllocator);
        arr.PushBack(pVec.y, *mAllocator);
        arr.PushBack(pVec.z, *mAllocator);
        arr.PushBack(pVec.w, *mAllocator);
        mCurrentValue->AddMember(rapidjson::Value(pKey, *mAllocator).Move(), arr, *mAllocator);
    }
    void JsonWriter::WriteVector3D(const char* pKey, const Vector3D& pVec)
    {
        rapidjson::Value arr(rapidjson::kArrayType);
        arr.PushBack(pVec.x, *mAllocator);
        arr.PushBack(pVec.y, *mAllocator);
        arr.PushBack(pVec.z, *mAllocator);
        mCurrentValue->AddMember(rapidjson::Value(pKey, *mAllocator).Move(), arr, *mAllocator);
    }
    void JsonWriter::WriteVector2D(const char* pKey, const Vector2D& pVec)
    {
        rapidjson::Value arr(rapidjson::kArrayType);
        arr.PushBack(pVec.x, *mAllocator);
        arr.PushBack(pVec.y, *mAllocator);
        mCurrentValue->AddMember(rapidjson::Value(pKey, *mAllocator).Move(), arr, *mAllocator);
    }
    void JsonWriter::WriteMatrice4DRow(const char* pKey, const Matrix4DRow& pMat)
    {
        rapidjson::Value arr(rapidjson::kArrayType);

        for (int i = 0; i < 4; ++i)
        {
            rapidjson::Value row(rapidjson::kArrayType);
            for (int j = 0; j < 4; ++j)
                row.PushBack(pMat.mat[i][j], *mAllocator);

            arr.PushBack(row, *mAllocator);
        }

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
