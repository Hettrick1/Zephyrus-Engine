#pragma once

#include "Scene.h"
#include "ShaderProgram.h"
#include "HudText.h"

class SplashScreen : public Scene
{
private:
	HudText* mZephyrusText;
	HudText* mStudioText;
	Scene* mNextScene;
	float mTime;
public:
	SplashScreen(Scene* pNextScene);
	~SplashScreen();
	void Start(IRenderer* pRenderer) override;
	void Update() override;
	void Render() override;
	void Close() override;
};