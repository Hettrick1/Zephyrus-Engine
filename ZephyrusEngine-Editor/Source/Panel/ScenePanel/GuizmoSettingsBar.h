#pragma once

#include "imgui.h"
#include "ImGuizmo.h"
#include "Vector3D.h"

class GuizmoSettingsBar
{
private:
    ImGuizmo::MODE mGuizmoMode = ImGuizmo::MODE::WORLD;
    ImGuizmo::OPERATION mGuizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
    Vector3D mSnap = { 1.0f, 1.0f, 1.0f };
    bool mUseSnap = false;
public:
    void DrawGuizmoSettingsBar();

    ImGuizmo::MODE GetGuizmoMode() const { return mGuizmoMode; }
    ImGuizmo::OPERATION GetGuizmoOperation() const { return mGuizmoOperation; }
    Vector3D GetSnap() const { return mSnap; }
    bool GetUseSnap() const { return mUseSnap; }
};
