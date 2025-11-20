#pragma once

#include <string>

#include "Interface/IShaderProgram.h"
#include "Interface/ITextureBase.h"
#include "Property/PropertyDescriptor.h"

#include "JSONUtils.h"
#include <unordered_map>

namespace Zephyrus::Material
{
    class IMaterial
    {
    public:
        virtual ~IMaterial() = default;

        virtual void SetVertexShader(Render::IShader* s) = 0;
        virtual void SetFragmentShader(Render::IShader* s) = 0;
        virtual void SetTessControlShader(Render::IShader* s) = 0;
        virtual void SetTessEvalShader(Render::IShader* s) = 0;
        virtual void SetGeometryShader(Render::IShader* s) = 0;

        virtual void RebuildShaderProgram() = 0;

        virtual Render::IShaderProgram* GetShaderProgram() = 0;

        virtual void SetTexture(const std::string& uniformName, Assets::ITextureBase* texture) = 0;
        virtual void RemoveTexture(const std::string& uniformName) = 0;

        virtual void SetProperty(const std::string& uniformName, float value) = 0;
        virtual void SetProperty(const std::string& uniformName, int value) = 0;
        virtual void SetProperty(const std::string& uniformName, const Vector2D& value) = 0;
        virtual void SetProperty(const std::string& uniformName, const Vector3D& value) = 0;
        virtual void SetProperty(const std::string& uniformName, const Vector4D& value) = 0;
        virtual void SetProperty(const std::string& uniformName, const Matrix4DRow& value) = 0;
        virtual void RemoveProperty(const std::string& uniformName) = 0;

        virtual std::unordered_map<std::string, float> GetFloatProperties() = 0;
        virtual std::unordered_map<std::string, int> GetIntProperties() = 0;
        virtual std::unordered_map<std::string, Vector2D> GetVec2Properties() = 0;
        virtual std::unordered_map<std::string, Vector3D> GetVec3Properties() = 0;
        virtual std::unordered_map<std::string, Vector4D> GetVec4Properties() = 0;
        virtual std::unordered_map<std::string, Matrix4DRow> GetMatrix4Properties() = 0;
        virtual std::unordered_map<std::string, Assets::ITextureBase*> GetTextureProperties() = 0;

        virtual void Use(const Matrix4DRow* world = nullptr, const Matrix4DRow* viewproj = nullptr) = 0;

        virtual void Serialize(Serialization::ISerializer& writer) const = 0;
        virtual void Deserialize(Serialization::IDeserializer& reader) = 0;

        virtual std::vector<PropertyDescriptor> GetProperties() = 0;
        
        virtual void SetFilePath(const std::string& filePath) = 0;
        virtual std::string GetFilePath() const = 0;
    };
}