#pragma once

namespace Zephyrus::Material
{
    class IBaseMaterialListener
    {
    public:
        virtual ~IBaseMaterialListener() = default;
        virtual void UpdateMaterial() = 0;
    };
}