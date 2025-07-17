#include "HudImage.h"

HudImage::HudImage(Texture& pTexture, Vector2D pPosition, Vector2D pSize, Vector2D pOrigin)
    : HudElement(), mTexture(pTexture), mWidth(pTexture.GetTextureSize().x * pSize.x), mHeight(pTexture.GetTextureSize().y * pSize.y), 
    mOrigin(pOrigin), mTint(Vector4D(1.0f, 1.0f, 1.0f, 1.0f))
{
    SetPosition(pPosition.x, pPosition.y);
}

void HudImage::Draw(RendererOpenGl& renderer)
{
    Rectangle rect;
    rect.dimensions = { mWidth, mHeight };
    rect.position = { mPositionX, mPositionY };
    glDisable(GL_CULL_FACE);
    renderer.DrawHudImage(mTexture, rect, mOrigin, mTint);
}

void HudImage::SetTexture(Texture& pTexture)
{
    mTexture = pTexture;
}

void HudImage::SetTint(Vector4D pColor)
{
    mTint = pColor;
}
