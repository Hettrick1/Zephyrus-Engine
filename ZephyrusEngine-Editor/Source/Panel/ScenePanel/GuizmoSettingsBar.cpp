#include "GuizmoSettingsBar.h"

#include <imgui.h>
#include <imgui_internal.h>

#include "EditorUI/ImGuiUtils.h"

void GuizmoSettingsBar::DrawGuizmoSettingsBar()
{
    if (ImGui::IsKeyPressed(ImGuiKey_W) && !ImGui::IsMouseDown(ImGuiMouseButton_Right))
    {
        mGuizmoOperation = 0;
    }
    if (ImGui::IsKeyPressed(ImGuiKey_E)&& !ImGui::IsMouseDown(ImGuiMouseButton_Right))
    {
        mGuizmoOperation = 1;
    }
    if (ImGui::IsKeyPressed(ImGuiKey_R)&& !ImGui::IsMouseDown(ImGuiMouseButton_Right))
    {
        mGuizmoOperation = 2;
    }

    const char* Items[] = {"Translate", "Rotate", "Scale"};

    ImGui::SetNextItemWidth(150);
    
    if (ImGui::Combo("##Operation", &mGuizmoOperation, Items, 3));

    ImGui::SameLine();
    ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical, 5);
    
    if (mGuizmoOperation != 2) // not equal to scale
    {
        ImGui::SameLine();
        if (ImGui::RadioButton("Local", mGuizmoMode == ImGuizmo::LOCAL))
            mGuizmoMode = ImGuizmo::LOCAL;
        ImGui::SameLine();
        if (ImGui::RadioButton("World", mGuizmoMode == ImGuizmo::WORLD))
            mGuizmoMode = ImGuizmo::WORLD;
    }
    else
    {
        mGuizmoMode = ImGuizmo::LOCAL;
    }
}
