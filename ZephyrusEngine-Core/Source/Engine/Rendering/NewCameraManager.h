#pragma once

#include "NewCameraComponent.h"

class NewCameraManager
{
private:
    std::vector<CameraComponent*> mCameras;
    CameraComponent* mActiveCamera = nullptr;
public:
    NewCameraManager() = default;
    ~NewCameraManager() = default;

    void AddCamera(CameraComponent* pCam);
    void RemoveCamera(CameraComponent* pCam);

    void SetActiveCamera(CameraComponent* pCam);
    inline CameraComponent* GetActiveCamera() const { return mActiveCamera; }

    void OnPlay();
    void OnStop();
    void Unload();

    void Update();
    void RenderActiveCamera();
};