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

    bool CustomImageButton(const char* id, ImTextureID pTexId, const ImVec2& pButtonSize, const ImVec2& pImageSize)
    {
        ImGui::PushID(id);

        bool hovered = false;
        bool held = false;
        
        if (ImGui::InvisibleButton("##btn", pButtonSize)) {
            ImGui::PopID();
            return true;
        }

        hovered = ImGui::IsItemHovered();
        held = ImGui::IsItemActive();

        ImVec2 p0 = ImGui::GetItemRectMin();
        ImVec2 p1 = ImGui::GetItemRectMax();
        
        ImU32 bg = IM_COL32(64, 64, 64, 255);  // normal

        if (held)
            bg = IM_COL32(150, 150, 150, 255); // Pressed
        else if (hovered)
            bg = IM_COL32(99, 99, 99, 255);    // Hover

        ImGui::GetWindowDrawList()->AddRectFilled(p0, p1, bg, 0.0f);
        
        ImVec2 pos(
            p0.x + (pButtonSize.x - pImageSize.x) * 0.5f,
            p0.y + (pButtonSize.y - pImageSize.y) * 0.5f
        );

        ImGui::GetWindowDrawList()->AddImage(
            pTexId,
            pos,
            ImVec2(pos.x + pImageSize.x, pos.y + pImageSize.y),
            ImVec2(0,0),
            ImVec2(1,1),
            IM_COL32_WHITE
        );

        ImGui::PopID();
        return false;
    }

    void DrawVerticalSplitterButton(float& v)
    {
        const float splitterWidth = 16.0f;

        ImGui::InvisibleButton("vsplitter", ImVec2(splitterWidth, -1));

        bool active = ImGui::IsItemActive();
        bool hovered = ImGui::IsItemHovered();

        if (active)
            v += ImGui::GetIO().MouseDelta.x;

        if (hovered)
            ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);

        ImVec2 min = ImGui::GetItemRectMin();
        ImVec2 max = ImGui::GetItemRectMax();
    
        ImDrawList* draw = ImGui::GetWindowDrawList();
	
        float centerX = (min.x + max.x) * 0.5f;

        draw->AddLine(
            ImVec2(centerX, min.y),
            ImVec2(centerX, max.y),
            IM_COL32(250, 194, 0, 155),
            1.5f
        );
    }

    void DrawSplitterButton(float& h)
    {
        const float splitterHeight = 16.0f;

        ImGui::InvisibleButton("hsplitter", ImVec2(-1, splitterHeight));

        bool active = ImGui::IsItemActive();
        bool hovered = ImGui::IsItemHovered();

        if (active)
            h += ImGui::GetIO().MouseDelta.y;

        if (hovered)
            ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeNS);

        ImVec2 min = ImGui::GetItemRectMin();
        ImVec2 max = ImGui::GetItemRectMax();
    
        ImDrawList* draw = ImGui::GetWindowDrawList();
	
        float centerY = (min.y + max.y) * 0.5f;

        draw->AddLine(
            ImVec2(min.x, centerY),
            ImVec2(max.x, centerY),
            IM_COL32(250, 194, 0, 155),
            1.5f
        );
    }
}
