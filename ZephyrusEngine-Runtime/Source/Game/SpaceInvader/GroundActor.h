#pragma once
#include "Actor.h"
#include "IActionListener.h"
#include "SpriteComponent.h"
#include "Physics/ICollisionListener.h"

class GroundActor : public Actor, public ICollisionListener
{
public:
	GroundActor(Vector3D pPos = 0, Vector3D pSize = 1, Quaternion pRotation = Quaternion(0,0));
	~GroundActor();
	void Start() override;
	void Update() override;
	void Destroy() override;
public:
	void OnTriggerEnter(ColliderComponent* collider, HitResult* infos) override;
	void OnTriggerStay(ColliderComponent* collider, HitResult* infos) override;
	void OnTriggerExit(ColliderComponent* collider, HitResult* infos) override;
};

