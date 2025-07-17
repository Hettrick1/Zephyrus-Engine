#pragma once

#include "IActionListener.h"
#include "Component.h"

class DoomMenuPC : public IActionListener, public Component
{
public:
	DoomMenuPC(Actor* pOwner, int pUpdateOrder = 100);
	DoomMenuPC() = delete;
	DoomMenuPC(const DoomMenuPC&) = delete;
	DoomMenuPC& operator=(const DoomMenuPC&) = delete;
	~DoomMenuPC();
public:
	void OnActionStarted(InputActions* action) override;
	void OnActionTriggered(InputActions* action) override;
	void OnActionEnded(InputActions* action) override;
public:
	void Update() override;
};

