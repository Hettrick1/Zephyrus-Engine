#pragma once
#include "HudElement.h"
#include "TextRenderer.h"
#include "RendererOpenGl.h"
#include "TextAlignementEnum.h"

/**
 * @brief HudText is a HUD element that displays text on the screen.
 * It allows customization of text content, position, scale, color, alignment, font, and shader program.
 */
class HudText :
    public HudElement
{
public:
    HudText(const std::string& text, float x, float y, float scale, Vector4D color,TextAlignment alignment = TextAlignment::LEFT, Font* pFont = nullptr);
    ~HudText();

    // Draws the text on the HUD using the specified renderer.
    void Draw(RendererOpenGl& renderer) override;

    // Sets the text to display.
    void SetText(std::string pText);

    // Sets the color of the text.
    void SetColor(Vector4D color);

    inline Vector4D GetColor() const { return mColor; }

    // Sets the shader program used for rendering the text.
    void SetShaderProgram(ShaderProgram* pShaderProgram);

private:
    std::string mText;
    float mScale;
    Vector4D mColor;
    TextAlignment mAlignment;
    Font* mFont;
    ShaderProgram* mShaderProgram;
};
