#pragma once

#include "Vector2D.h"
#include "Vector3D.h"
#include "Matrix4DRow.h"
#include "Shaders/ShaderProgram.h"
#include "Window.h"
#include "TextAlignementEnum.h"
#include <map>
#include "Font.h"

/**
 * @brief Singleton class responsible for rendering text using OpenGL.
 * It manages text drawing, alignment, and font handling.
 */
class TextRenderer
{
public:
    static TextRenderer& Instance();

    // Initializes the text renderer with the given window
    bool Init(Window& pWindow);

    void RenderText(std::string text, float x, float y, float scale, Vector4D color, Font pFont, TextAlignment alignment, ShaderProgram* pShaderProgram = nullptr);

    // Computes the width of the given text string at the specified scale and font
    float ComputeTextWidth(const std::string& text, float scale, Font pFont);
private:
    ~TextRenderer();
    TextRenderer() = default;
    TextRenderer(const TextRenderer&) = delete;
    TextRenderer& operator=(const TextRenderer&) = delete;
    unsigned int VAO, VBO;
    Window* mWindow;
    Shader mVertexShader, mFragmentShader;
    ShaderProgram mShaderProgram;
    Matrix4DRow mProjection;
};
