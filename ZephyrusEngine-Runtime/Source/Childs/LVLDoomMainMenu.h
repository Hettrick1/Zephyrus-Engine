#pragma once

#include "Scene.h"
#include "Shaders/ShaderProgram.h"
#include "HudElements/HudText.h"
#include "HudElements/HudImage.h"

class LVLDoomMainMenu : public Scene
{
public:
	LVLDoomMainMenu();
	~LVLDoomMainMenu();
	void Start(IRenderer* renderer) override;
	void Update() override;
	void Render() override;
	void Close() override;
private:
	HudImage* mDoomMenu;
	HudText* mPressEnter;
	float mTimer;
};