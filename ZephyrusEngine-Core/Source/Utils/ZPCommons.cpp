#include "ZPCommons.h"

Scene* Zephyrus::Commons::GetCurrentScene()
{
    auto activeScene = Zephyrus::Scenes::SceneManager::ActiveScene;
    if (activeScene)
    {
        return activeScene;
    }
    return nullptr;
}

Zephyrus::ActorComponent::Actor* Zephyrus::Commons::GetPlayerRef()
{
    auto playerRef = Zephyrus::Scenes::SceneManager::ActiveScene->GetPlayerRef();
    if (playerRef)
    {
        return playerRef;
    }
    return nullptr;
}

Zephyrus::ActorComponent::CameraComponent* Zephyrus::Commons::GetCurrentCamera()
{
    auto currentCamera = Zephyrus::Scenes::SceneManager::ActiveScene->GetCameraManager()->GetActiveCamera();
    if (currentCamera)
    {
        return currentCamera;
    }
    return nullptr;
}

CameraManager* Zephyrus::Commons::GetCameraManager()
{
    auto cameraManager = Zephyrus::Scenes::SceneManager::ActiveScene->GetCameraManager();
    if (cameraManager)
    {
        return cameraManager;
    }
    return nullptr;
}

Zephyrus::Physics::PhysicWorld* Zephyrus::Commons::GetPhysicWorld()
{
    auto physicWorld = Zephyrus::Scenes::SceneManager::ActiveScene->GetPhysicWorld();
    if (physicWorld)
    {
        return physicWorld;
    }
    return nullptr;
}
