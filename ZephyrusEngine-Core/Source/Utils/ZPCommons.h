#pragma once

#include "SceneManager.h"
#include "Scene.h"
#include "CameraManager.h"
#include "CameraComponent.h"
#include "Actor.h"
#include "Bullet/PhysicWorld.h"

namespace Zephyrus::Commons
{
	extern Scene* GetCurrentScene();
	extern Zephyrus::ActorComponent::Actor* GetPlayerRef();
	extern Zephyrus::ActorComponent::CameraComponent* GetCurrentCamera();
	extern CameraManager* GetCameraManager();
	extern Zephyrus::Physics::PhysicWorld* GetPhysicWorld();
}