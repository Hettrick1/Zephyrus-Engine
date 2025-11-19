#pragma once
#include <cstdint>

namespace Zephyrus::ComponentFlags
{
    enum Flags : uint8_t
    {
        None            = 0,
        No_Edit         = 1 << 0,
        No_Parent       = 1 << 1,
        Hidden_In_Game  = 1 << 2,
    };

    // Check if a flag is set
    constexpr bool Has(uint8_t mask, Flags flag)
    {
        return (mask & flag) != 0;
    }

    // Add a flag
    constexpr void Add(uint8_t& mask, Flags flag)
    {
        mask |= flag;
    }

    // Remove a flag
    constexpr void Remove(uint8_t& mask, Flags flag)
    {
        mask &= ~flag;
    }
}
