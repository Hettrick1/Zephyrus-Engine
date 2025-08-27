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
}
