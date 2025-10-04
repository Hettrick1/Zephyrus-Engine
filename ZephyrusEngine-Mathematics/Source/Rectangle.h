#pragma once

#include "Vector2D.h"

struct Rectangle
{
    Vector2D position;
    Vector2D dimensions;

    bool operator==(const Rectangle& other) const
    {
        return position.x == other.position.x &&
            position.y == other.position.y &&
            dimensions.x == other.dimensions.x &&
            dimensions.y == other.dimensions.y;
    }

    bool operator!=(const Rectangle& other) const
    {
        return !(*this == other);
    }

    static const Rectangle Null;
};
