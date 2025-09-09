#pragma once

#include "Scene.h"
#include "ShaderProgram.h"
#include "HudText.h"

class SplashScreen : public Scene
{
private:
	HudText* mZephyrusText;
	HudText* mStudioText;
	std::string mNextScene;
	float mTime;
public:
	SplashScreen(const std::string& pNextScene);
	~SplashScreen();
	void Start() override;
	void Update() override;
	void Render() override;
	void Close() override;
};