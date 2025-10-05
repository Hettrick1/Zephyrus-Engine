#pragma once

namespace Zephyrus::Factory
{
    class ComponentFactory;
    class PrefabFactory;
    class SceneFactory;
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
    virtual Zephyrus::Factory::PrefabFactory* GetPrefabFactory() = 0;
    virtual Zephyrus::Factory::SceneFactory* GetSceneFactory() = 0;
    virtual Zephyrus::Scenes::Scene* GetActiveScene() = 0;
};