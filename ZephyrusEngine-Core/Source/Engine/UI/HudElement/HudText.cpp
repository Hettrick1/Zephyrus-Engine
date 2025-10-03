#include "HudText.h"
#include "Assets.h"

using Zephyrus::Assets::AssetsManager;

HudText::HudText(const std::string& pText, const Vector2D& pPos, float pScale, const Vector4D& pColor, TextAlignment pAlignment, Zephyrus::Assets::Font* pFont)
    : HudElement(), mText(pText), mScale(pScale), mColor(pColor), mAlignment(pAlignment), mFont(pFont), mShaderProgram(nullptr)
{
    if (pFont == nullptr) {
        mFont = AssetsManager::LoadFont("RoadPixel.ttf", "RoadPixel");
    }
    SetPosition(pPos);
}

HudText::~HudText()
{
}

void HudText::Draw(RendererOpenGl& pRenderer)
{
    TextRenderer::Instance().RenderText(mText, mPosition, mScale, mColor, *mFont, mAlignment, mShaderProgram);
}

void HudText::SetText(std::string pText)
{
    mText = pText;
}

void HudText::SetColor(Vector4D pColor)
{
    mColor = pColor;
}

void HudText::SetShaderProgram(ShaderProgram* pShaderProgram)
{
    mShaderProgram = pShaderProgram;
}
