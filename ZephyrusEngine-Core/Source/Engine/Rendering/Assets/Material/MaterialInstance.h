#pragma once

#include "JSONUtils.h"
#include "Material/IMaterial.h"
#include <map>
#include "IBaseMaterialListener.h"

namespace Zephyrus::Material
{
    class MaterialInstance : public IBaseMaterialListener
    {
    private:
        IMaterial* mBaseMaterial{ nullptr };
        std::map<std::string, float> mFloatOverrides;
        std::map<std::string, int> mIntOverrides;
        std::map<std::string, Vector2D> mVector2DOverrides;
        std::map<std::string, Vector3D> mVector3DOverrides;
        std::map<std::string, Vector4D> mVector4DOverrides;
        std::map<std::string, Matrix4DRow> mMatrix4DOverrides;
        std::map<std::string, Assets::ITextureBase*> mTextureOverrides;
    public:
        MaterialInstance() = default;
        ~MaterialInstance() override = default;
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

        inline std::map<std::string, float> GetFloatOverrides() { return mFloatOverrides; }
        inline std::map<std::string, int> GetIntOverrides() { return mIntOverrides; }
        inline std::map<std::string, Vector2D> GetVec2Overrides() { return mVector2DOverrides; }
        inline std::map<std::string, Vector3D> GetVec3Overrides() { return mVector3DOverrides; }
        inline std::map<std::string, Vector4D> GetVec4Overrides() { return mVector4DOverrides; }
        inline std::map<std::string, Matrix4DRow> GetMatrix4Overrides() { return mMatrix4DOverrides; }
        inline std::map<std::string, Assets::ITextureBase*> GetTextureOverrides() { return mTextureOverrides; }

        inline IMaterial* GetBaseMaterial() const { return mBaseMaterial; }

        void UpdateMaterial() override;

        template<typename T>
        void UpdateProperties(std::map<std::string, T>& container, const std::vector<std::pair<std::string, T>>& properties)
        {
            std::map<std::string, T> tempContainer;
            for (auto& [name, value] : properties)
            {
                if (container.contains(name))
                {
                    tempContainer[name] = container[name];
                    continue;
                }
                tempContainer[name]  = value;
            }
            container = tempContainer;
        }
    };
}
