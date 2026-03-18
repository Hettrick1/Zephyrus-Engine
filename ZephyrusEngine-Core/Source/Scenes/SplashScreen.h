#pragma once

#include "Scene.h"
#include "Interface/IShaderProgram.h"
#include "HudText.h"

namespace Zephyrus::Scenes {
	class SplashScreen : public Scene
	{
	private:
		Zephyrus::UI::HudText* mZephyrusText;
		Zephyrus::UI::HudText* mStudioText;
		Zephyrus::UI::HudText* mVersionText;
		std::string mNextScene;
		float mTime;
	public:
		SplashScreen(ISceneContext* pContext, const std::string& pNextScene);
		~SplashScreen() override;
		void Start() override;
		void Update(float pDetltaTime) override;
		void Render() override;
		void Close() override;
	};
}