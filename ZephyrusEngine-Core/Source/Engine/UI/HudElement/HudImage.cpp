#include "HudImage.h"

HudImage::HudImage(Zephyrus::Assets::Texture& pTexture, const Vector2D& pPosition, const Vector2D& pSize, const Vector2D& pOrigin)
    : HudElement(), mTexture(pTexture), mWidth(pTexture.GetTextureSize().x * pSize.x), mHeight(pTexture.GetTextureSize().y * pSize.y), 
    mOrigin(pOrigin), mTint(Vector4D(1.0f, 1.0f, 1.0f, 1.0f))
{
    SetPosition(pPosition);
}

void HudImage::Draw(RendererOpenGl& pRenderer)
{
    Rectangle rect;
    rect.dimensions = { mWidth, mHeight };
    rect.position = mPosition;
    glDisable(GL_CULL_FACE);
    pRenderer.DrawHudImage(mTexture, rect, mOrigin, mTint);
}

void HudImage::SetTexture(const Zephyrus::Assets::Texture& pTexture)
{
    mTexture = pTexture;
}

void HudImage::SetTint(const Vector4D& pColor)
{
    mTint = pColor;
}
