#pragma once

#include "imgui.h"
#include "ImGuizmo.h"
#include "Vector3D.h"

class GizmoSettingsBar
{
private:
    ImGuizmo::MODE mGuizmoMode = ImGuizmo::MODE::WORLD;
    ImGuizmo::MODE mOldGuizmoMode = ImGuizmo::MODE::WORLD;
    int mGuizmoOperation = 0;
    bool mUseSnap = false;
    
    Vector3D mSnap = { 0.0f, 45.0f, 1.0f };
    
public:
    void DrawGizmoSettingsBar();
    void DrawSnap();

    ImGuizmo::MODE GetGuizmoMode() const { return mGuizmoMode; }
    ImGuizmo::OPERATION GetGuizmoOperation() const
    {
        switch (mGuizmoOperation)
        {
            case 0:
            return ImGuizmo::TRANSLATE;
            case 1:
            return ImGuizmo::ROTATE;
            case 2:
            return ImGuizmo::SCALE;
            default:
            return ImGuizmo::TRANSLATE;
        }
    }
    Vector3D GetSnap() const;
    void SetSnap(float value);
    bool GetUseSnap() const { return mUseSnap; }
};
