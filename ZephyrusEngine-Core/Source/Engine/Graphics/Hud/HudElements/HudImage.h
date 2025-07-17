#pragma once
#include "HudElement.h"
#include "RendererOpenGl.h"
#include "Texture.h"
#include "Vector4D.h"

/**
 * @brief Represents a HUD element that displays an image with optional tint and origin.
 * Allows setting the texture, position, size, origin, and tint color for rendering on the HUD.
 */
class HudImage :
    public HudElement
{
public:
    HudImage(Texture& pTexture, Vector2D pPosition, Vector2D pSize = 1, Vector2D pOrigin = 0);
        
    void Draw(RendererOpenGl& renderer) override; // Draws the image on the HUD using the given renderer.

    void SetTexture(Texture& pTexture); // Sets the texture to display.

    void SetTint(Vector4D pColor); // Sets the tint color to apply to the image.

private:
    Texture mTexture;
    float mWidth, mHeight;
    Vector2D mOrigin;
    Vector4D mTint;
};
