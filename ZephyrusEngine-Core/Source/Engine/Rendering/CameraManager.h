#pragma once

#include "CameraComponent.h"

namespace zpActorComp = Zephyrus::ActorComponent;

class CameraManager
{
private:
    std::vector<zpActorComp::CameraComponent*> mCameras;
    zpActorComp::CameraComponent* mActiveCamera = nullptr;
public:
    CameraManager() = default;
    ~CameraManager() = default;

    void AddCamera(zpActorComp::CameraComponent* pCam);
    void RemoveCamera(zpActorComp::CameraComponent* pCam);

    void SetActiveCamera(zpActorComp::CameraComponent* pCam);
    inline zpActorComp::CameraComponent* GetActiveCamera() const { return mActiveCamera; }

    void OnPlay();
    void OnStop();
    void Unload();

    void Update();
    void RenderActiveCamera();
};