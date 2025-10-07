#pragma once
#include "HudElement.h"
#include "TextRenderer.h"
#include "RendererOpenGl.h"
#include "TextAlignementEnum.h"

/**
 * @brief HudText is a HUD element that displays text on the screen.
 * It allows customization of text content, position, scale, color, alignment, font, and shader program.
 */

using Zephyrus::Render::ShaderProgram;
namespace Zephyrus::UI {
    class HudText : public HudElement
    {
    private:
        std::string mText;
        float mScale = 0;
        Vector4D mColor;
        TextAlignment mAlignment;
        Assets::IFont* mFont = nullptr;
        ShaderProgram* mShaderProgram = nullptr;
    public:
        HudText(ISceneContext* pContext, const std::string& pText, const Vector2D& pPos, float pScale, const Vector4D& pColor, TextAlignment pAlignment = TextAlignment::LEFT, Assets::IFont* pFont = nullptr);
        ~HudText();

        // Draws the text on the HUD using the specified renderer.
        void Draw(Zephyrus::Render::IRenderer& pRenderer) override;

        // Sets the text to display.
        void SetText(std::string pText);

        // Sets the color of the text.
        void SetColor(Vector4D pColor);

        inline Vector4D GetColor() const { return mColor; }

        // Sets the shader program used for rendering the text.
        void SetShaderProgram(ShaderProgram* pShaderProgram);
    };
}