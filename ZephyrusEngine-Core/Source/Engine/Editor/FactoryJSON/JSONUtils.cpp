#include "JSONUtils.h"
#include <string>
#include "Log.h"

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
