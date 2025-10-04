#pragma once

#include "Scene.h"
#include "ShaderProgram.h"
#include "HudText.h"

namespace Zephyrus::Scenes {
	class SplashScreen : public Scene
	{
	private:
		Zephyrus::UI::HudText* mZephyrusText;
		Zephyrus::UI::HudText* mStudioText;
		std::string mNextScene;
		float mTime;
	public:
		SplashScreen(const std::string& pNextScene);
		~SplashScreen();
		void Start() override;
		void Update(float pDetltaTime) override;
		void Render() override;
		void Close() override;
	};
}