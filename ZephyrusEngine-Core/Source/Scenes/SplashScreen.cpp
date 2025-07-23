#include "SplashScreen.h"
#include "CameraActor.h"
#include "MeshComponent.h"
#include "Assets.h"
#include "HudElement.h"
#include "Timer.h"
#include "TextRenderer.h"
#include "HudManager.h" 
#include "SceneManager.h"


SplashScreen::SplashScreen(Scene* pNextScene)
	: Scene(), mZephyrusText(nullptr), mStudioText(nullptr), mTime(0), mNextScene(pNextScene)
{
}

SplashScreen::~SplashScreen()
{
	delete mZephyrusText;
	delete mStudioText;
}

void SplashScreen::Start(IRenderer* pRenderer)
{
	Scene::Start(pRenderer);

	mTime = 0;

	CameraActor* cam = new CameraActor();

	Shader vert;
	vert.Load("VertFrag/TextSplashScreen.vert", ShaderType::VERTEX);
	Shader frag;
	frag.Load("VertFrag/TextSplashScreen.frag", ShaderType::FRAGMENT);

	ShaderProgram* shaderProgram = new ShaderProgram();
	shaderProgram->Compose({ &vert, &frag });

	mZephyrusText = new HudText("Zephyrus", 0, 0, 2.0f, Vector4D(1.0, 0.81176f, 0.0, 0.0), TextAlignment::CENTER, Assets::LoadFont("../Imports/Fonts/Chopsic.otf", "Chopsic"));
	mZephyrusText->SetShaderProgram(shaderProgram);

	mStudioText = new HudText("Engine", 0, -200, 0.5f, Vector4D(1, 1, 1, 0), TextAlignment::CENTER, Assets::LoadFont("../Imports/Fonts/Chopsic.otf", "Chopsic"));
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
		SceneManager::LoadScene(mNextScene);
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
