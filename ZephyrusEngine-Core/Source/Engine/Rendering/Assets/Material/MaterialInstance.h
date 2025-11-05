#pragma once

#include "IMaterialInstance.h"
#include <unordered_map>

namespace Zephyrus::Material
{
    class MaterialInstance : public IMaterialInstance
    {
    private:
        IMaterial* mBaseMaterial;
        std::unordered_map<std::string, float> mFloatOverrides;
        std::unordered_map<std::string, int> mIntOverrides;
        std::unordered_map<std::string, Vector2D> mVector2DOverrides;
        std::unordered_map<std::string, Vector3D> mVector3DOverrides;
        std::unordered_map<std::string, Vector4D> mVector4DOverrides;
        std::unordered_map<std::string, Matrix4DRow> mMatrix4DOverrides;
        std::unordered_map<std::string, Assets::ITextureBase*> mTextureOverrides;
    public:
        MaterialInstance(IMaterial* base);
        ~MaterialInstance() = default;
        void SetMaterial(IMaterial* newMaterial) override;
        void ClearOverride(const std::string& name) override;
        void ClearOverrides() override;

        void SetFloat(const std::string& name, float value) override;
        void SetInt(const std::string& name, int value) override;
        void SetVector2D(const std::string& name, const Vector2D& value) override;
        void SetVector3D(const std::string& name, const Vector3D& value) override;
        void SetVector4D(const std::string& name, const Vector4D& value) override;
        void SetMatrix4DRow(const std::string& name, const Matrix4DRow& value) override;
        void SetTexture(const std::string& name, Assets::ITextureBase* texture) override;

        void Use(const Matrix4DRow* world = nullptr, const Matrix4DRow* viewproj = nullptr) override;

        void Serialize(Serialization::ISerializer& writer) const override;
        void Deserialize(Serialization::IDeserializer& reader) override;

        inline IMaterial* GetBaseMaterial() const { return mBaseMaterial; }
    };
}