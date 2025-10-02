#pragma once

#include "CameraComponent.h"

using Zephyrus::ActorComponent::CameraComponent;
using Zephyrus::ActorComponent::CameraUsage;

class CameraManager
{
private:
    std::vector<CameraComponent*> mCameras;
    CameraComponent* mActiveCamera = nullptr;
public:
    CameraManager() = default;
    ~CameraManager() = default;

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