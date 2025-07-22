#pragma once

#include "Vector2D.h"
#include "Vector4D.h"
#include "RendererOpenGl.h"

/**
 * @brief Base class for all HUD elements.
 * Provides position management and an interface for drawing HUD elements.
 */
class HudElement
{
public:
    HudElement();
    virtual ~HudElement() = default;
    virtual void Draw(RendererOpenGl& pRenderer) = 0;

    // Sets the position of the HUD element.
    void SetPosition(float pX, float pY);

    inline Vector2D GetPosition() const { return { mPositionX, mPositionY }; }

protected:
    float mPositionX = 0, mPositionY = 0;
};
