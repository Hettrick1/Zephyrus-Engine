#include "SplashScreen.h"
#include "MeshComponent.h"
#include "Assets.h"
#include "HudElement.h"
#include "Timer.h"
#include "TextRenderer.h"
#include "HudManager.h" 
#include "SceneManager.h"
#include "PrefabFactory.h"


SplashScreen::SplashScreen(const std::string& pNextScene)
	: Scene("SplashScreen"), mZephyrusText(nullptr), mStudioText(nullptr), mTime(0), mNextScene(pNextScene)
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

	auto cam = PrefabFactory::CreateActorFromPrefab("CameraActor");

	Shader vert = *Assets::LoadShader("VertFrag/TextSplashScreen.vert", ShaderType::VERTEX, "TextSplashScreenVert");
	Shader frag = *Assets::LoadShader("VertFrag/TextSplashScreen.frag", ShaderType::FRAGMENT, "TextSplashScreenFrag");

	mZephyrusText = new HudText("Zephyrus", Vector2D(0), 2.0f, Vector4D(1.0, 0.81176f, 0.0, 0.0), TextAlignment::CENTER, Assets::LoadFont("Chopsic.otf", "Chopsic"));
	mZephyrusText->SetShaderProgram(Assets::LoadShaderProgram({ &vert, &frag }, "splashScreenSP"));

	mStudioText = new HudText("Engine", Vector2D(0.0f, -200.0f), 0.5f, Vector4D(1, 1, 1, 0), TextAlignment::CENTER, Assets::LoadFont("Chopsic.otf", "Chopsic"));
	SDL_SetRelativeMouseMode(SDL_TRUE);
}

void SplashScreen::Update()
{
	Scene::Update();
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
		SceneManager::LoadSceneWithFile(mNextScene, nullptr, false);
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
