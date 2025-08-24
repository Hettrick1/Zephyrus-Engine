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
protected:
    Vector2D mPosition;
    float mDrawOrder = 0;
public:
    HudElement();
    virtual ~HudElement() = default;
    virtual void Draw(RendererOpenGl& pRenderer) = 0;

    // Sets the position of the HUD element.
    void SetPosition(const Vector2D& pPosition);

    inline Vector2D GetPosition() const { return mPosition; }

    void SetDrawOrder(float pDrawOrder);
    inline float GetDrawOrder() const { return mDrawOrder; }
};
