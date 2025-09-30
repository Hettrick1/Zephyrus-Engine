#include "ZPCommons.h"

Scene* Zephyrus::Commons::GetCurrentScene()
{
    auto activeScene = SceneManager::ActiveScene;
    if (activeScene)
    {
        return activeScene;
    }
    return nullptr;
}

Actor* Zephyrus::Commons::GetPlayerRef()
{
    auto playerRef = SceneManager::ActiveScene->GetPlayerRef();
    if (playerRef)
    {
        return playerRef;
    }
    return nullptr;
}

CameraComponent* Zephyrus::Commons::GetCurrentCamera()
{
    auto currentCamera = SceneManager::ActiveScene->GetCameraManager()->GetActiveCamera();
    if (currentCamera)
    {
        return currentCamera;
    }
    return nullptr;
}

CameraManager* Zephyrus::Commons::GetCameraManager()
{
    auto cameraManager = SceneManager::ActiveScene->GetCameraManager();
    if (cameraManager)
    {
        return cameraManager;
    }
    return nullptr;
}

PhysicWorld* Zephyrus::Commons::GetPhysicWorld()
{
    auto physicWorld = SceneManager::ActiveScene->GetPhysicWorld();
    if (physicWorld)
    {
        return physicWorld;
    }
    return nullptr;
}
