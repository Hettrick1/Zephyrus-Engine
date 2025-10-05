#include "SplashScreen.h"
#include "MeshComponent.h"
#include "Assets.h"
#include "HudElement.h"
#include "Timer.h"
#include "TextRenderer.h"
#include "HudManager.h" 
#include "SceneManager.h"
#include "PrefabFactory.h"

using Zephyrus::Assets::AssetsManager;
using Zephyrus::Render::Shader;
using Zephyrus::Render::ShaderType;

namespace Zephyrus::Scenes {
	SplashScreen::SplashScreen(ISceneContext* pContext, const std::string& pNextScene)
		: Scene(pContext, "SplashScreen"), mZephyrusText(nullptr), mStudioText(nullptr), mTime(0), mNextScene(pNextScene)
	{
	}

	SplashScreen::~SplashScreen()
	{
		delete mZephyrusText;
		delete mStudioText;
	}

	void SplashScreen::Start()
	{
		Scene::Start();

		mTime = 0;

		Shader vert = *AssetsManager::LoadShader("VertFrag/TextSplashScreen.vert", ShaderType::VERTEX, "TextSplashScreenVert");
		Shader frag = *AssetsManager::LoadShader("VertFrag/TextSplashScreen.frag", ShaderType::FRAGMENT, "TextSplashScreenFrag");

		mZephyrusText = new Zephyrus::UI::HudText(mContext, "Zephyrus", Vector2D(0), 2.0f, Vector4D(1.0, 0.81176f, 0.0, 0.0), Zephyrus::UI::TextAlignment::CENTER, AssetsManager::LoadFont("Chopsic.otf", "Chopsic"));
		mZephyrusText->SetShaderProgram(AssetsManager::LoadShaderProgram({ &vert, &frag }, "splashScreenSP"));

		mStudioText = new Zephyrus::UI::HudText(mContext, "Engine", Vector2D(0.0f, -200.0f), 0.5f, Vector4D(1, 1, 1, 0), Zephyrus::UI::TextAlignment::CENTER, AssetsManager::LoadFont("Chopsic.otf", "Chopsic"));
		SDL_SetRelativeMouseMode(SDL_TRUE);
	}

	void SplashScreen::Update(float pDetltaTime)
	{
		Scene::Update(pDetltaTime);
		mTime += Timer::deltaTime;
		if (mTime > 0.2f && mTime < 0.7)
		{
			float alpha1 = (mTime - 0.2f) * 2;
			mZephyrusText->SetColor(Vector4D(mZephyrusText->GetColor().xyz, alpha1));
		}
		if (mTime > 2.1f && mTime < 2.6)
		{
			float alpha2 = (mTime - 2.1f) * 2;
			mStudioText->SetColor(Vector4D(mStudioText->GetColor().xyz, alpha2));
		}
		if (mTime > 3.0f)
		{
			GetAllActors()[0]->GetSceneContext()->LoadSceneWithFile(mNextScene, nullptr, true);
		}
	}

	void SplashScreen::Render()
	{
		Scene::Render();
	}

	void SplashScreen::Close()
	{
		Scene::Close();
	}
}