#pragma once

#include <string>

#include "Interface/IShaderProgram.h"
#include "Interface/ITextureBase.h"

#include "JSONUtils.h"

#include <variant>

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

        virtual void SetTexture(const std::string& uniformName, Assets::ITextureBase* texture) = 0;
        virtual void RemoveTexture(const std::string& uniformName) = 0;

        virtual void SetProperty(const std::string& uniformName, float value) = 0;
        virtual void SetProperty(const std::string& uniformName, int value) = 0;
        virtual void SetProperty(const std::string& uniformName, const Vector2D& value) = 0;
        virtual void SetProperty(const std::string& uniformName, const Vector3D& value) = 0;
        virtual void SetProperty(const std::string& uniformName, const Vector4D& value) = 0;
        virtual void SetProperty(const std::string& uniformName, const Matrix4DRow& value) = 0;
        virtual void RemoveProperty(const std::string& uniformName) = 0;

        virtual void Use() = 0;

        virtual void Serialize(Serialization::ISerializer& writer) const = 0;
        virtual void Deserialize(Serialization::IDeserializer& value) = 0;
    };
}