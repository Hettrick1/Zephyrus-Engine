#pragma once
#include "Actor.h"
#include "IActionListener.h"
#include "FlipbookComponent.h"
#include "Physics/ICollisionListener.h"

class SpaceInvaderPlayer : public Actor, public ICollisionListener
{
public:
	SpaceInvaderPlayer();
	~SpaceInvaderPlayer();
	void Start() override;
	void Update() override;
	void Destroy() override;
public :
	void OnTriggerEnter(ColliderComponent* collider, HitResult* infos) override;
	void OnTriggerStay(ColliderComponent* collider, HitResult* infos) override;
	void OnTriggerExit(ColliderComponent* collider, HitResult* infos) override;
private:
};

