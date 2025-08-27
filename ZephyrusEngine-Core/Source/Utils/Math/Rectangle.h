#pragma once

#include <SDL_rect.h>
#include "Vector2D.h"

struct Rectangle
{
    Vector2D position;
    Vector2D dimensions;

    SDL_Rect ToSdlRect() const
    {
        return SDL_Rect{
            static_cast<int>(position.x),
            static_cast<int>(position.y),
            static_cast<int>(dimensions.x),
            static_cast<int>(dimensions.y) };
    }

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
