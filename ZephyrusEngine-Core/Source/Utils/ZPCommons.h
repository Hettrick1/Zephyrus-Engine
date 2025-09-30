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
	extern Actor* GetPlayerRef();
	extern CameraComponent* GetCurrentCamera();
	extern CameraManager* GetCameraManager();
	extern PhysicWorld* GetPhysicWorld();
}