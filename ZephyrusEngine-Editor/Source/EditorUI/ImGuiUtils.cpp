#include "ImGuiUtils.h"

namespace ZP::UI
{
    Fonts gFonts;

    bool ImageTextButton(ImTextureID pTexId, const ImVec2& pButtonSize, const ImVec2& pImageSize, const ImVec2& pButtonPos, const char* pLabel, ImFont* pFont)
    {
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        if (window->SkipItems)
        {
            return false;
        }

        ImGui::PushFont(pFont);

        ImVec2 textSize = ImGui::CalcTextSize(pLabel);
        ImVec2 buttonSize = pButtonSize;

        ImVec2 pos = pButtonPos;
        ImRect bb(pos, ImVec2(pos.x + buttonSize.x, pos.y + buttonSize.y));

        ImGui::ItemSize(bb);
        if (!ImGui::ItemAdd(bb, window->GetID(pLabel)))
        {
            ImGui::PopFont();
            return false;
        }

        ImGui::SetCursorScreenPos(pos);

        bool pressed = ImGui::InvisibleButton(pLabel, buttonSize);
        bool hovered = ImGui::IsItemHovered();
        bool held = ImGui::IsItemActive();

        ImU32 bgColor;
        if (held)
        {
            bgColor = IM_COL32(150, 150, 150, 255);
        }
        else if (hovered) 
        { 
            bgColor = IM_COL32(100, 100, 100, 255); 
        }
        else
        {
            bgColor = IM_COL32(60, 60, 60, 255);
        }

        window->DrawList->AddRectFilled(bb.Min, bb.Max, bgColor, 4.0f);

        ImVec2 imagePos(pos.x + pButtonSize.x * 0.15, pos.y + (pButtonSize.y - pImageSize.y) * 0.5);

        float textSpaceAvail = pButtonSize.x * 0.85 + pImageSize.x - 20;

        ImVec2 textPos(imagePos.x + textSpaceAvail * 0.5 - textSize.x * 0.5, pos.y + (buttonSize.y - textSize.y) / 2);

        ImGui::SetCursorScreenPos(imagePos);
        ImGui::Image(pTexId, pImageSize);
        ImGui::SameLine();
        ImGui::SetCursorScreenPos(textPos);
        ImGui::TextUnformatted(pLabel);
        ImGui::PopFont();

        return pressed;
    }
}