#include "pch.h"
#include "HudImage.h"

namespace Zephyrus::UI {
    HudImage::HudImage(ISceneContext* pContext, Assets::ITexture2D* pTexture, const Vector2D& pPosition, const Vector2D& pSize, const Vector2D& pOrigin)
        : HudElement(pContext), mTexture(pTexture), mWidth(pTexture->GetWidth() * pSize.x), mHeight(pTexture->GetHeight() * pSize.y),
        mOrigin(pOrigin), mTint(Vector4D(1.0f, 1.0f, 1.0f, 1.0f))
    {
        SetPosition(pPosition);
    }

    void HudImage::Draw(Zephyrus::Render::IRenderer& pRenderer)
    {
        Rectangle2D rect;
        rect.dimensions = { mWidth, mHeight };
        rect.position = mPosition;
        glDisable(GL_CULL_FACE);
        pRenderer.DrawHudImage(mTexture, rect, mOrigin, mTint);
    }

    void HudImage::SetTexture(Assets::ITexture2D* pTexture)
    {
        mTexture = pTexture;
    }

    void HudImage::SetTint(const Vector4D& pColor)
    {
        mTint = pColor;
    }
}