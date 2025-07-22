#include "HudText.h"
#include "Assets.h"

HudText::HudText(const std::string& pText, float pX, float pY, float pScale, Vector4D pColor, TextAlignment pAlignment, Font* pFont)
    : HudElement(), mText(pText), mScale(pScale), mColor(pColor), mAlignment(pAlignment), mFont(pFont), mShaderProgram(nullptr)
{
    if (pFont == nullptr) {
        mFont = Assets::LoadFont("../Imports/Fonts/RoadPixel.ttf", "RoadPixel");
    }
    SetPosition(pX, pY);
}

HudText::~HudText()
{
    if (mShaderProgram)
    {
        delete mShaderProgram;
    }
}

void HudText::Draw(RendererOpenGl& pRenderer)
{
    TextRenderer::Instance().RenderText(mText, mPositionX, mPositionY, mScale, mColor, *mFont, mAlignment, mShaderProgram);
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
