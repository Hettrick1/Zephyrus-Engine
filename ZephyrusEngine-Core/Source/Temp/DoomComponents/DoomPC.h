#pragma once

#include "IActionListener.h"
#include "RigidbodyComponent.h"
#include "Component.h"

class DoomPC : public IActionListener, public Component
{
public:
	DoomPC(Actor* pOwner, int pUpdateOrder = 100);
	DoomPC() = delete;
	DoomPC(const DoomPC&) = delete;
	DoomPC& operator=(const DoomPC&) = delete;
	~DoomPC();

	void Deserialize(const rapidjson::Value& pData) override;

	static Component* Create(Actor* pOwner) { return new DoomPC(pOwner); }
public:
	void OnActionStarted(InputActions* action) override;
	void OnActionTriggered(InputActions* action) override;
	void OnActionEnded(InputActions* action) override;
public :
	void Update() override;
private:
	RigidbodyComponent* playerRbRef;
	bool goRight, goLeft, goForward, goBackward;
};

