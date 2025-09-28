#pragma once

#include "Scene.h"
#include "ShaderProgram.h"
#include "HudText.h"
#include "HudImage.h"

class LVLDoomMainMenu : public Scene
{
public:
	LVLDoomMainMenu();
	~LVLDoomMainMenu();
	void Start() override;
	void Update(float pDetltaTime) override;
	void Render() override;
	void Close() override;
private:
	HudImage* mDoomMenu;
	HudText* mPressEnter;
	float mTimer;
};