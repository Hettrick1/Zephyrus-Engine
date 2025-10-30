#pragma once
#include "HudElement.h"
#include "RendererOpenGl.h"
#include "Interface/ITexture2D.h"
#include "Vector4D.h"

/**
 * @brief Represents a HUD element that displays an image with optional tint and origin.
 * Allows setting the texture, position, size, origin, and tint color for rendering on the HUD.
 */
namespace Zephyrus::UI {
    class HudImage :
        public HudElement
    {
    private:
        Assets::ITexture2D* mTexture;
        float mWidth, mHeight;
        Vector2D mOrigin;
        Vector4D mTint;
    public:
        HudImage(ISceneContext* pContext, Assets::ITexture2D* pTexture, const Vector2D& pPosition, const Vector2D& pSize = 1, const Vector2D& pOrigin = 0);

        void Draw(Zephyrus::Render::IRenderer& pRenderer) override; // Draws the image on the HUD using the given renderer.

        void SetTexture(Assets::ITexture2D* pTexture); // Sets the texture to display.

        void SetTint(const Vector4D& pColor); // Sets the tint color to apply to the image.
    };
}
