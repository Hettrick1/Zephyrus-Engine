#pragma once

#include "imgui.h"
#include "ImGuizmo.h"
#include "Vector3D.h"

class GuizmoSettingsBar
{
private:
    ImGuizmo::MODE mGuizmoMode = ImGuizmo::MODE::WORLD;
    ImGuizmo::MODE mOldGuizmoMode = ImGuizmo::MODE::WORLD;
    int mGuizmoOperation = 0;
    bool mUseSnap = false;

    Vector3D mSnap = { 1.0f, 1.0f, 1.0f };
    
public:
    void DrawGuizmoSettingsBar();

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
    float* GetSnap();
    void SetSnap(float value);
    bool GetUseSnap() const { return mUseSnap; }
};
