#pragma once

#include "CameraComponent.h"

class ISceneContext;

using Zephyrus::ActorComponent::CameraComponent;
using Zephyrus::ActorComponent::CameraUsage;

class CameraManager
{
private:
    std::vector<CameraComponent*> mCameras;
    ISceneContext* mContext{ nullptr };
    CameraComponent* mActiveCamera{ nullptr };
public:
    explicit CameraManager(ISceneContext* pContext);
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