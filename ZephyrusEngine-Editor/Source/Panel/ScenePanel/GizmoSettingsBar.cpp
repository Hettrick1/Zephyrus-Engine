#include "GizmoSettingsBar.h"

#include <imgui.h>
#include <imgui_internal.h>

#include "EditorUI/ImGuiUtils.h"
#include "Timer.h"
#include "Maths.h"

void GizmoSettingsBar::DrawGizmoSettingsBar()
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
        ImGui::SameLine();
        ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical, 5);
    }
    else
    {
        mGuizmoMode = ImGuizmo::LOCAL;
    }
    ImGui::SameLine();
    
    DrawSnap();
    
    ImGui::SameLine();
    ImGui::SeparatorEx(ImGuiSeparatorFlags_Vertical, 5);
    ImGui::SameLine();

    float deltaTime = Timer::mDeltaTimeDebug;
    deltaTime = zpMaths::roundFloat(deltaTime, 4);

    std::string fpsText = "fps : " + std::to_string(Timer::mFPS) + "\t | \t" + std::to_string(deltaTime) + " ms";

    ImVec4 color = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);

    if (Timer::mFPS < 20)
    {
        color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
    }
    if (Timer::mFPS < 40 && Timer::mFPS > 20)
    {
        color = ImVec4(1.0f, 0.5f, 0.0f, 1.0f);
    }

    ImGui::TextColored(color, fpsText.c_str());
}

void GizmoSettingsBar::DrawSnap()
{
    float snap = GetSnap().x;

    ImGui::Text("Snap");

    ImGui::SameLine();
    ImGui::SetNextItemWidth(150);
    
    switch (mGuizmoOperation)
    {
    case 0: // translate
        if (ImGui::DragFloat("##Snap", &snap, 0.1f, 0.0f, 100.0f, "%.2f"))
        {
            SetSnap(snap);
        }
        break;
    case 1: // rotate
        if (ImGui::DragFloat("##Snap", &snap, 1.0f, 0.0f, 90.0f, "%.0f"))
        {
            SetSnap(snap);
        }
        break;
    case 2: // scale
        if (ImGui::DragFloat("##Snap", &snap, 0.1f, 0.0f, 10.0f, "%.2f"))
        {
            SetSnap(snap);
        }
        break;
    default:
        break;
    }

    if (ImGui::IsItemHovered())
    {
        ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
    }
    
}

Vector3D GizmoSettingsBar::GetSnap() const
{
    Vector3D snap = Vector3D::zero;
    switch (mGuizmoOperation)
    {
    case 0: // translate
        snap = Vector3D(mSnap.x);
        break;
    case 1: // rotate
        snap = Vector3D(mSnap.y);
        break;
    case 2: // scale
        snap = Vector3D(mSnap.z);
        break;
    default:
        break;
    }
    return snap;
}

void GizmoSettingsBar::SetSnap(float value)
{
    switch (mGuizmoOperation)
    {
    case 0: // translate
        mSnap.x = value;
        break;
    case 1: // rotate
        mSnap.y = value;
        break;
    case 2: // scale
        mSnap.z = value;
        break;
    default:
        break;
    }
}
