#pragma once

#include "NewCameraComponent.h"

class NewCameraManager
{
private:
    std::vector<NewCameraComponent*> mCameras;
    NewCameraComponent* mActiveCamera = nullptr;
public:
    NewCameraManager() = default;
    ~NewCameraManager() = default;

    void AddCamera(NewCameraComponent* pCam);
    void RemoveCamera(NewCameraComponent* pCam);

    void SetActiveCamera(NewCameraComponent* pCam);
    inline NewCameraComponent* GetActiveCamera() const { return mActiveCamera; }

    void OnPlay();
    void OnStop();
    void Unload();

    void Update();
    void RenderActiveCamera();
};