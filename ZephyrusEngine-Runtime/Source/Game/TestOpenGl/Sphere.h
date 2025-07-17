#pragma once

#include "Actor.h"
#include "SpriteComponent.h"
#include "Physics/ICollisionListener.h"

class Sphere : public Actor, public ICollisionListener
{
public:
	Sphere(Vector3D pPos = 0, Vector3D pSize = 1, Quaternion pRotation = Quaternion(0, 0));
	~Sphere();
	void Start() override;
	void Update() override;
	void Destroy() override;
public:
	void OnTriggerEnter(ColliderComponent* collider, HitResult* infos) override;
	void OnTriggerStay(ColliderComponent* collider, HitResult* infos) override;
	void OnTriggerExit(ColliderComponent* collider, HitResult* infos) override;
private :
};

