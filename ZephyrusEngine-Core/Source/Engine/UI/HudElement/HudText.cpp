#include "HudText.h"
#include "AssetsManager.h"

using Zephyrus::Assets::AssetsManager;

namespace Zephyrus::UI {
    HudText::HudText(ISceneContext* pContext, const std::string& pText, const Vector2D& pPos, float pScale, const Vector4D& pColor, TextAlignment pAlignment, Assets::IFont* pFont)
        : HudElement(pContext), mText(pText), mScale(pScale), mColor(pColor), mAlignment(pAlignment), mFont(pFont), mShaderProgram(nullptr)
    {
        if (pFont == nullptr) {
            mFont = AssetsManager::LoadFont("RoadPixel.ttf", "RoadPixel");
        }
        SetPosition(pPos);
    }

    HudText::~HudText()
    {
    }

    void HudText::Draw(Zephyrus::Render::IRenderer& pRenderer)
    {
        Zephyrus::Render::TextRenderer::Instance().RenderText(mText, mPosition, mScale, mColor, mFont, mAlignment, mShaderProgram);
    }

    void HudText::SetText(std::string pText)
    {
        mText = pText;
    }

    void HudText::SetColor(Vector4D pColor)
    {
        mColor = pColor;
    }

    void HudText::SetShaderProgram(Render::IShaderProgram* pShaderProgram)
    {
        mShaderProgram = pShaderProgram;
    }
}