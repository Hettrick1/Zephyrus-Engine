#pragma once

#include <string>

namespace Zephyrus::Factory
{
    class ComponentFactory;
    class IPrefabFactory;
    class ISceneFactory;
}

namespace Zephyrus::Physics
{
    class PhysicWorld;
}
namespace Zephyrus::Render 
{
    class IRenderer;
}
namespace Zephyrus::Scenes
{
    class Scene;
}

class CameraManager;

class ISceneContext {
public:
    virtual ~ISceneContext() = default;
    virtual Zephyrus::Physics::PhysicWorld* GetPhysicsWorld() = 0;
    virtual Zephyrus::Render::IRenderer* GetRenderer() = 0;
    virtual CameraManager* GetCameraManager() = 0;
    virtual Zephyrus::Factory::ComponentFactory* GetComponentFactory() = 0;
    virtual Zephyrus::Factory::IPrefabFactory* GetPrefabFactory() = 0;
    virtual Zephyrus::Factory::ISceneFactory* GetSceneFactory() = 0;
    virtual Zephyrus::Scenes::Scene* GetActiveScene() = 0;
    virtual void LoadSceneWithFile(const std::string& pFilePath, Zephyrus::Render::IRenderer* pRenderer = nullptr, bool pCallPostStart = true) = 0;
    virtual void SetSceneLoaded(bool pSceneLoaded) = 0;
};