#pragma once
#include <cstdint>

namespace Zephyrus::PropertyFlags
{
    enum Flags : uint16_t
    {
        None                = 0,
        Fixed_Size          = 1 << 0,
        Read_Only           = 1 << 1,
        Advanced            = 1 << 2,
        Condition           = 1 << 3,
        Clamp               = 1 << 4,
        Range               = 1 << 5,
        Tooltip             = 1 << 6,
        DisplayName         = 1 << 7,
        Category            = 1 << 8,
        Hidden_In_Editor    = 1 << 9,
        Disable_In_Editor   = 1 << 10,
    };

    // Check if a flag is set
    constexpr bool HasFlag(uint16_t mask, Flags flag)
    {
        return (mask & flag) != 0;
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
