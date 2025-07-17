#include "TextRenderer.h"

#include "glew.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include "Log.h"
#include <iostream>
#include <string>
#include "Timer.h"

TextRenderer& TextRenderer::Instance()
{
    static TextRenderer instance;
    return instance;
}

bool TextRenderer::Init(Window& pWindow)
{
    mWindow = &pWindow;
    mVertexShader.Load("TextRenderer.vert", ShaderType::VERTEX);
    mFragmentShader.Load("TextRenderer.frag", ShaderType::FRAGMENT);
    mShaderProgram.Compose({ &mVertexShader, &mFragmentShader });
    mProjection = Matrix4DRow::CreateOrtho(static_cast<float>(pWindow.GetDimensions().x), static_cast<float>(pWindow.GetDimensions().y), 0.000001f, 100000);
    mShaderProgram.Use();
    mShaderProgram.setMatrix4Row("projection", mProjection);

    // configure VAO/VBO for texture quads
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    Log::Info("TextRenderer initialized successfully !");
    return true;
}

void TextRenderer::RenderText(std::string text, float x, float y, float scale, Vector4D color, Font pFont ,TextAlignment alignment, ShaderProgram* pShaderProgram)
{
    float textWidth = ComputeTextWidth(text, scale, pFont);

    ShaderProgram* shaderProgram = pShaderProgram == nullptr ? &mShaderProgram : pShaderProgram;

    if (alignment == TextAlignment::CENTER)
    {
        x -= textWidth / 2.0f;
    }
    else if (alignment == TextAlignment::RIGHT)
    {
        x -= textWidth;
    }

    // activate corresponding render state	
    shaderProgram->Use();
    shaderProgram->setMatrix4Row("projection", mProjection);
    shaderProgram->setVector4f("textColor", color);
    shaderProgram->setFloat("time", SDL_GetTicks());
    shaderProgram->setFloat("screenWidth", mWindow->GetDimensions().x);
    shaderProgram->setFloat("screenHeight", mWindow->GetDimensions().y);
    shaderProgram->setMatrix4Row("projection", mProjection);
    glActiveTexture(GL_TEXTURE0); 
    glBindVertexArray(VAO); 

    // iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = pFont.GetCharacters()[*c];

        float xpos = x + ch.Bearing.x * scale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        float w = ch.Size.x * scale;
        float h = ch.Size.y * scale;
        // update VBO for each character
        float vertices[6][4] = {
            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos,     ypos,       0.0f, 1.0f },
            { xpos + w, ypos,       1.0f, 1.0f },

            { xpos,     ypos + h,   0.0f, 0.0f },
            { xpos + w, ypos,       1.0f, 1.0f },
            { xpos + w, ypos + h,   1.0f, 0.0f }
        };
        // render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

float TextRenderer::ComputeTextWidth(const std::string& text, float scale, Font pFont)
{
    float width = 0.0f;
    for (char c : text)
    {
        if (pFont.GetCharacters().find(c) != pFont.GetCharacters().end())
        {
            width += (pFont.GetCharacters()[c].Advance >> 6) * scale;  // Convertit 1/64 pixels en pixels
        }
    }
    return width;
}

TextRenderer::~TextRenderer()
{
}

