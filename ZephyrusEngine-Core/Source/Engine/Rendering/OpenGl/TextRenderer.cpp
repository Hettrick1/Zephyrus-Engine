#include "pch.h"
#include "TextRenderer.h"

#include "glew.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include "Log.h"
#include "Timer.h"
#include "AssetsManager.h"
#include "Interface/IFont.h"
#include <iostream>
#include <string>

using Zephyrus::Assets::AssetsManager;

namespace Zephyrus::Render {
    TextRenderer& TextRenderer::Instance()
    {
        static TextRenderer instance;
        return instance;
    }

    bool TextRenderer::Init(Window& pWindow)
    {
        mWindow = &pWindow;
        mVertexShader = AssetsManager::LoadShader("TextRenderer.vert", ShaderType::VERTEX, "TextRendererVert");
        mFragmentShader = AssetsManager::LoadShader("TextRenderer.frag", ShaderType::FRAGMENT, "TextRendererFrag");
        mShaderProgram = AssetsManager::LoadShaderProgram({ mVertexShader, mFragmentShader }, "textRendererSP");
        mProjection = Matrix4DRow::CreateOrtho(static_cast<float>(pWindow.GetDimensions().x), static_cast<float>(pWindow.GetDimensions().y), 0.000001f, 100000);
        mShaderProgram->Use();
        mShaderProgram->setMatrix4Row("projection", mProjection);

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

        ZP_CORE_INFO("TextRenderer initialized successfully !");
        return true;
    }

    void TextRenderer::RenderText(std::string pText, const Vector2D& pPos, float pScale, Vector4D pColor, Assets::IFont* pFont, TextAlignment pAlignment, IShaderProgram* pShaderProgram)
    {
        if (!pFont) return;

        float textWidth = ComputeTextWidth(pText, pScale, pFont);

        IShaderProgram* shaderProgram = pShaderProgram == nullptr ? mShaderProgram : pShaderProgram;

        Vector2D position = pPos;

        if (pAlignment == TextAlignment::CENTER)
        {
            position.x -= textWidth / 2.0f;
        }
        else if (pAlignment == TextAlignment::RIGHT)
        {
            position.x -= textWidth;
        }
	
        shaderProgram->Use();
        shaderProgram->setMatrix4Row("projection", mProjection);
        shaderProgram->setVector4f("textColor", pColor);
        shaderProgram->setFloat("time", static_cast<float>(glfwGetTime() * 1000)); // miliseconds
        shaderProgram->setFloat("screenWidth", mWindow->GetDimensions().x);
        shaderProgram->setFloat("screenHeight", mWindow->GetDimensions().y);
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(VAO);

        // iterate through all characters
        std::string::const_iterator c;
        for (c = pText.begin(); c != pText.end(); c++)
        {
            const auto& characters = pFont->GetCharacters();
            auto it = characters.find(*c);
            if (it != characters.end())
            {
                const Zephyrus::Assets::CharacterInfo& ch = it->second;
                float xpos = position.x + ch.Bearing.x * pScale;
                float ypos = position.y - (ch.Size.y - ch.Bearing.y) * pScale;

                float w = ch.Size.x * pScale;
                float h = ch.Size.y * pScale;

                float vertices[6][4] = {
                    { xpos,     ypos + h,   0.0f, 0.0f },
                    { xpos,     ypos,       0.0f, 1.0f },
                    { xpos + w, ypos,       1.0f, 1.0f },

                    { xpos,     ypos + h,   0.0f, 0.0f },
                    { xpos + w, ypos,       1.0f, 1.0f },
                    { xpos + w, ypos + h,   1.0f, 0.0f }
                };
                pFont->BindCharacterTexture(*c);
                
                glBindBuffer(GL_ARRAY_BUFFER, VBO);
                glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

                glBindBuffer(GL_ARRAY_BUFFER, 0);
                glDrawArrays(GL_TRIANGLES, 0, 6);
                position.x += (ch.Advance >> 6) * pScale; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
            }
        }
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    float TextRenderer::ComputeTextWidth(const std::string& pText, float pScale, const Assets::IFont* pFont)
    {
        float width = 0.0f;
        const auto& chars = pFont->GetCharacters();
        for (char c : pText)
        {
            auto it = chars.find(c);
            if (it != chars.end())
            {
                width += (it->second.Advance >> 6) * pScale;
            }
        }
        return width;
    }
}
