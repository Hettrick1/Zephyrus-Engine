#include "HudText.h"
#include "Assets.h"

HudText::HudText(const std::string& text, float x, float y, float scale, Vector4D color, TextAlignment alignment, Font* pFont)
    : HudElement(), mText(text), mScale(scale), mColor(color), mAlignment(alignment), mFont(pFont), mShaderProgram(nullptr)
{
    if (pFont == nullptr) {
        mFont = Assets::LoadFont("../Imports/Fonts/RoadPixel.ttf", "RoadPixel");
    }
    SetPosition(x, y);
}

HudText::~HudText()
{
    if (mShaderProgram)
    {
        delete mShaderProgram;
    }
}

void HudText::Draw(RendererOpenGl& renderer)
{
    TextRenderer::Instance().RenderText(mText, mPositionX, mPositionY, mScale, mColor, *mFont, mAlignment, mShaderProgram);
}

void HudText::SetText(std::string pText)
{
    mText = pText;
}

void HudText::SetColor(Vector4D color)
{
    mColor = color;
}

void HudText::SetShaderProgram(ShaderProgram* pShaderProgram)
{
    mShaderProgram = pShaderProgram;
}
