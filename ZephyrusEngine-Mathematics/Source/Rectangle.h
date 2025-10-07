#pragma once

#include "Vector2D.h"

struct Rectangle2D
{
    Rectangle2D() = default;
    Vector2D position{0};
    Vector2D dimensions{0};

    bool operator==(const Rectangle2D& other) const
    {
        return position.x == other.position.x &&
            position.y == other.position.y &&
            dimensions.x == other.dimensions.x &&
            dimensions.y == other.dimensions.y;
    }

    bool operator!=(const Rectangle2D& other) const
    {
        return !(*this == other);
    }

    static const Rectangle2D Null;
};
