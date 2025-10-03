#pragma once

#include "Vector2D.h"
#include "Vector3D.h"
#include "Matrix4DRow.h"
#include "ShaderProgram.h"
#include "Window.h"
#include "TextAlignementEnum.h"
#include <map>
#include "Font.h"

/**
 * @brief Singleton class responsible for rendering text using OpenGL.
 * It manages text drawing, alignment, and font handling.
 */
using Zephyrus::Application::Window;

class TextRenderer
{
private:
    ~TextRenderer();
    TextRenderer() = default;
    TextRenderer(const TextRenderer&) = delete;
    TextRenderer& operator=(const TextRenderer&) = delete;
    unsigned int VAO = 0, VBO = 0;
    Window* mWindow = nullptr;
    Shader mVertexShader, mFragmentShader;
    ShaderProgram mShaderProgram;
    Matrix4DRow mProjection;
public:
    static TextRenderer& Instance();

    // Initializes the text renderer with the given window
    bool Init(Window& pWindow);

    void RenderText(std::string pText, const Vector2D& pPos, float pScale, Vector4D pColor, Font pFont, TextAlignment pAlignment, ShaderProgram* pShaderProgram = nullptr);

    // Computes the width of the given text string at the specified scale and font
    float ComputeTextWidth(const std::string& pText, float pScale, Font pFont);
};
