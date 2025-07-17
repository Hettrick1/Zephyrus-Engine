#pragma once

#include "IActionListener.h"
#include "RigidbodyComponent.h"
#include "Component.h"

class DoomPlayer;

class DoomPC : public IActionListener, public Component
{
public:
	DoomPC(Actor* pOwner, int pUpdateOrder = 100);
	DoomPC() = delete;
	DoomPC(const DoomPC&) = delete;
	DoomPC& operator=(const DoomPC&) = delete;
	~DoomPC();
public:
	void OnActionStarted(InputActions* action) override;
	void OnActionTriggered(InputActions* action) override;
	void OnActionEnded(InputActions* action) override;
public :
	void Update() override;
	void SetPlayerRef(DoomPlayer* playerRef);
private:
	RigidbodyComponent* playerRbRef;
	DoomPlayer* mPlayerRef;
	bool goRight, goLeft, goForward, goBackward;
};

