#include "pch.h"
#include "SplashScreen.h"
#include "MeshComponent.h"
#include "AssetsManager.h"
#include "HudElement.h"
#include "Timer.h"
#include "TextRenderer.h"
#include "HudManager.h" 
#include "SceneManager.h"
#include "PrefabFactory.h"
#include "Utils.h"
#include "EngineContentIds.h"

using Zephyrus::Assets::AssetsManager;
using Zephyrus::Render::IShader;
using Zephyrus::Render::ShaderType;

namespace Zephyrus::Scenes {
	SplashScreen::SplashScreen(ISceneContext* pContext, const std::string& pNextScene)
		: Scene(pContext, "SplashScreen"), mZephyrusText(nullptr), mStudioText(nullptr), mVersionText(nullptr), mTime(0), mNextScene(pNextScene)
	{
	}

	SplashScreen::~SplashScreen()
	{
	}

	void SplashScreen::Start()
	{
		Scene::Start();

		mTime = 0.0f;

		Render::IShader* vert = AssetsManager::GetInstance().LoadShader(SH_SPLASH_SCREEN_VERT, ShaderType::VERTEX);
		Render::IShader* frag = AssetsManager::GetInstance().LoadShader(SH_SPLASH_SCREEN_FRAG, ShaderType::FRAGMENT);

		mZephyrusText = new Zephyrus::UI::HudText(mContext, "Zephyrus", Vector2D(0), 2.0f, Vector4D(1.0, 0.81176f, 0.0, 0.0), Zephyrus::UI::TextAlignment::CENTER, AssetsManager::GetInstance().LoadFont(FONT_CHOPSIC));
		mZephyrusText->SetShaderProgram(AssetsManager::GetInstance().LoadShaderProgram({ vert, frag }, "splashScreenSP"));

		mStudioText = new Zephyrus::UI::HudText(mContext, "Engine", Vector2D(0.0f, -200.0f), 0.5f, Vector4D(1, 1, 1, 0), Zephyrus::UI::TextAlignment::CENTER, AssetsManager::GetInstance().LoadFont(FONT_CHOPSIC));

		std::string versionEngineText = "Version - ";
		versionEngineText.append(ENGINE_VERSION);
		mVersionText = new Zephyrus::UI::HudText(mContext, versionEngineText, Vector2D(-1850.0f, 950.0f), 0.3f, Vector4D(1, 1, 1, 1), Zephyrus::UI::TextAlignment::LEFT, AssetsManager::GetInstance().LoadFont(FONT_CHOPSIC));
		
		mInputManager->SetCursorVisible(false);
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
			mContext->LoadSceneFromFileId(mNextScene, nullptr, true);
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