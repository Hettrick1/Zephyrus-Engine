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
        if (mGuizmoMode != mOldGuizmoMode)
        {
            mGuizmoMode = mOldGuizmoMode;
        }
        ImGui::SameLine();
        if (ImGui::RadioButton("Local", mGuizmoMode == ImGuizmo::LOCAL))
        {
            mGuizmoMode = ImGuizmo::LOCAL;
            mOldGuizmoMode = mGuizmoMode;
        }
        ImGui::SameLine();
        if (ImGui::RadioButton("World", mGuizmoMode == ImGuizmo::WORLD))
        {
            mGuizmoMode = ImGuizmo::WORLD;
            mOldGuizmoMode = mGuizmoMode;
        }
    }
    else
    {
        mGuizmoMode = ImGuizmo::LOCAL;
    }

    ImGui::SameLine();

    float snap = GetSnap();
    
    if (ImGui::SliderFloat("Snap", &snap, 0.0f, 10.0f, "%.3f"))
    {
        SetSnap(snap);
    }
    
}

float GuizmoSettingsBar::GetSnap() const
{
    switch (mGuizmoOperation)
    {
    case 0 :
        return mSnap.x;
    case 1 :
        return mSnap.y;
    case 2 :
        return mSnap.z;
    default:
        return 0.0f;
    }
}

void GuizmoSettingsBar::SetSnap(float value)
{
    switch (mGuizmoOperation)
    {
    case 0 :
        mSnap.x = value;
        break;
    case 1 :
        mSnap.y = value;
        break;
    case 2 :
        mSnap.z = value;
        break;
    default:
        break;
    }
}
