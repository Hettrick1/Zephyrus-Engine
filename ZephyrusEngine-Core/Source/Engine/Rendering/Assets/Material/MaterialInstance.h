#pragma once

#include "JSONUtils.h"
#include "Material/IMaterial.h"
#include <unordered_map>

namespace Zephyrus::Material
{
    class MaterialInstance
    {
    private:
        IMaterial* mBaseMaterial{ nullptr };
        std::unordered_map<std::string, float> mFloatOverrides;
        std::unordered_map<std::string, int> mIntOverrides;
        std::unordered_map<std::string, Vector2D> mVector2DOverrides;
        std::unordered_map<std::string, Vector3D> mVector3DOverrides;
        std::unordered_map<std::string, Vector4D> mVector4DOverrides;
        std::unordered_map<std::string, Matrix4DRow> mMatrix4DOverrides;
        std::unordered_map<std::string, Assets::ITextureBase*> mTextureOverrides;
    public:
        MaterialInstance() = default;
        ~MaterialInstance() = default;
        void SetMaterial(IMaterial* newMaterial);
        void ClearOverride(const std::string& name);
        void ClearOverrides();

        void SetFloat(const std::string& name, float value);
        void SetInt(const std::string& name, int value);
        void SetVector2D(const std::string& name, const Vector2D& value);
        void SetVector3D(const std::string& name, const Vector3D& value);
        void SetVector4D(const std::string& name, const Vector4D& value);
        void SetMatrix4DRow(const std::string& name, const Matrix4DRow& value);
        void SetTexture(const std::string& name, Assets::ITextureBase* texture);

        void Use(const Matrix4DRow* world = nullptr, const Matrix4DRow* viewproj = nullptr);

        void Serialize(Serialization::ISerializer& writer) const;
        void Deserialize(Serialization::IDeserializer& reader);

        inline std::unordered_map<std::string, float> GetFloatOverrides() { return mFloatOverrides; }
        inline std::unordered_map<std::string, int> GetIntOverrides() { return mIntOverrides; }
        inline std::unordered_map<std::string, Vector2D> GetVec2Overrides() { return mVector2DOverrides; }
        inline std::unordered_map<std::string, Vector3D> GetVec3Overrides() { return mVector3DOverrides; }
        inline std::unordered_map<std::string, Vector4D> GetVec4Overrides() { return mVector4DOverrides; }
        inline std::unordered_map<std::string, Matrix4DRow> GetMatrix4Overrides() { return mMatrix4DOverrides; }
        inline std::unordered_map<std::string, Assets::ITextureBase*> GetTextureOverrides() { return mTextureOverrides; }

        inline IMaterial* GetBaseMaterial() const { return mBaseMaterial; }
    };
}