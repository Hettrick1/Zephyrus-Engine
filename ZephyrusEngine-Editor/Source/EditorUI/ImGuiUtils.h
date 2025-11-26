#pragma once

#include "imgui.h"
#include "imgui_internal.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"

#include <string>

namespace ZP::UI
{
    struct Fonts {
        ImFont* small;
        ImFont* medium;
        ImFont* large;
    };
    extern Fonts gFonts;

	bool ImageTextButton(ImTextureID pTexId, const ImVec2& pButtonSize, const ImVec2& pImageSize, const ImVec2& pButtonPos, const char* pLabel, ImFont* pFont);

    bool CustomImageButton(const char* id, ImTextureID pTexId, const ImVec2& pButtonSize, const ImVec2& pImageSize);

    void DrawSplitterButton(float& h);
    void DrawVerticalSplitterButton(float& v);
}
