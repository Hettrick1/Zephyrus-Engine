#pragma once
#include <cstdint>

namespace Zephyrus::PropertyFlags
{
    enum Flags : uint16_t
    {
        None                = 0,
        No_Edit             = 1 << 0,
        Fixed_Size          = 1 << 1,
        Read_Only           = 1 << 2,
        Advanced            = 1 << 3,
        Collapsed           = 1 << 4,
        Clamp               = 1 << 5,
        Positive            = 1 << 6,
        Range               = 1 << 7,
        No_Serialize        = 1 << 8,
        Editor_Only         = 1 << 9,
        Tooltip             = 1 << 10,
        DisplayName         = 1 << 11,
        Category            = 1 << 12,
        Hidden_In_Editor    = 1 << 13,
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
