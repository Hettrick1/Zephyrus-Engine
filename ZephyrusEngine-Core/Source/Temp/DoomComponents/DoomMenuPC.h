#pragma once

#include "IActionListener.h"
#include "Component.h"
#include "HudText.h"
#include "HudImage.h"

class DoomMenuPC : public IActionListener, public Component
{
private:
	HudImage* mDoomMenu = nullptr;
	HudText* mPressEnter = nullptr;
	float mTimer = 0;
public:
	DoomMenuPC(Actor* pOwner, int pUpdateOrder = 100);
	DoomMenuPC() = delete;
	DoomMenuPC(const DoomMenuPC&) = delete;
	DoomMenuPC& operator=(const DoomMenuPC&) = delete;
	~DoomMenuPC();

	void Deserialize(const rapidjson::Value& pData) override;
	void Serialize(Serialization::Json::JsonWriter& pWriter) override;

	void OnStart() override;

	static Component* Create(Actor* pOwner) { return new DoomMenuPC(pOwner); }

public:
	void OnActionStarted(InputActions* action) override;
	void OnActionTriggered(InputActions* action) override;
	void OnActionEnded(InputActions* action) override;
public:
	void Update() override;
};

