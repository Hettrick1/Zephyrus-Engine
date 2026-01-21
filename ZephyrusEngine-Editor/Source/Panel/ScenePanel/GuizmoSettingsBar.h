#pragma once

#include "imgui.h"
#include "ImGuizmo.h"
#include "Vector3D.h"

class GuizmoSettingsBar
{
private:
    ImGuizmo::MODE mGuizmoMode = ImGuizmo::MODE::WORLD;
    int mGuizmoOperation = 0;
    Vector3D mSnap = { 1.0f, 1.0f, 1.0f };
    bool mUseSnap = false;
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
    Vector3D GetSnap() const { return mSnap; }
    bool GetUseSnap() const { return mUseSnap; }
};
