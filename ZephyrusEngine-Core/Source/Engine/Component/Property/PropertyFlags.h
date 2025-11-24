#pragma once
#include <cstdint>

namespace Zephyrus::PropertyFlags
{
    enum Flags : uint16_t
    {
        None                = 0,
        Fixed_Size          = 1 << 0,
        Advanced            = 1 << 1,
        Condition           = 1 << 2,
        Clamp               = 1 << 3,
        Range               = 1 << 4,
        Tooltip             = 1 << 5,
        DisplayName         = 1 << 6,
        Category            = 1 << 7,
        Hidden_In_Editor    = 1 << 8,
        Disable_In_Editor   = 1 << 9,
    };

    // Check if a flag is set
    constexpr bool HasFlag(uint16_t mask, Flags flag)
    {
        return (mask & flag) != 0;
    }
    
    constexpr bool HasNotFlag(uint16_t mask, Flags flag)
    {
        return (mask & flag) == 0;
    }

    // Add a flag
    constexpr void AddFlag(uint16_t& mask, Flags flag)
    {
        mask |= flag;
    }

    // Remove a flag
    constexpr void RemoveFlag(uint16_t& mask, Flags flag)
    {
        mask &= ~flag;
    }
}
