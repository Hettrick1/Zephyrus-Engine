#pragma once
#include "Actor.h"
#include "SpriteComponent.h"
#include "Physics/ICollisionListener.h"

class AmoPickup : public Actor, public ICollisionListener
{
public:
	AmoPickup(Vector3D pPos = 0, Vector3D pSize = 1, Quaternion pRotation = Quaternion(0, 0));
	~AmoPickup();
	void Start() override;
	void Update() override;
	void Destroy() override;
public:
	void OnTriggerEnter(ColliderComponent* collider, HitResult* infos) override;
	void OnTriggerStay(ColliderComponent* collider, HitResult* infos) override;
	void OnTriggerExit(ColliderComponent* collider, HitResult* infos) override;
};