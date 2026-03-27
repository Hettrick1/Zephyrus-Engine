#pragma once

#include <string_view>
#include <string>

inline constexpr std::string_view ENGINE_VERSION = "1.0.0";

inline std::string StringifyFileSize(uint64_t size)
{
    static constexpr char FILE_SIZE_UNITS[4][4]
    {
        "B", "KiB", "MiB", "GiB"
    };
    int counter = 0;
    double mantissa;
    for (mantissa = static_cast<double>(size); mantissa >= 1024.0; ++counter)
    {
        mantissa /= 1024.0;
    }

    std::string result = std::to_string(static_cast<int>(std::ceil(mantissa)));
    result.reserve(result.size() + 5);
    result.push_back(' ');
    if (counter < 4)
    {
        auto first = FILE_SIZE_UNITS[counter];
        result.insert(result.end(), first, first + sizeof(FILE_SIZE_UNITS[counter]));
    }

    return result;
}