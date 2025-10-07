#pragma once

#include <string>
#include <cstdint>

namespace Zephyrus::Assets {
    class IMesh
    {
    public:
        virtual ~IMesh() = default;
        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
        virtual uint32_t GetVertexCount() const = 0;
        virtual std::string GetFilePath() const = 0;
    };
}