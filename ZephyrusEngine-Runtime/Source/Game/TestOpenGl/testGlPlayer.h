#pragma once
#include "Actor.h"
#include "IActionListener.h"
#include "FlipbookComponent.h"
#include "Physics/ICollisionListener.h"

class testGlPlayer : public Actor
{
public:
	testGlPlayer();
	~testGlPlayer();
	void Start() override;
	void Update() override;
	void Destroy() override;
private:
};

