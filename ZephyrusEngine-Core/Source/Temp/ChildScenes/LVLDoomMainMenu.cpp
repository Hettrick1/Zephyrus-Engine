#include "LVLDoomMainMenu.h"
#include "MeshComponent.h"
#include "Assets.h"
#include "HudElement.h"
#include "Timer.h"
#include "TextRenderer.h"
#include "HudManager.h" 
#include "SceneManager.h"
#include "LVLDoom.h"
#include "EmptyActor.h"
#include "PrefabFactory.h"

LVLDoomMainMenu::LVLDoomMainMenu()
	: Scene(), mDoomMenu(nullptr), mPressEnter(nullptr), mTimer(0)
{
}

LVLDoomMainMenu::~LVLDoomMainMenu()
{

}

void LVLDoomMainMenu::Start()
{
	Scene::Start();
	auto actor = PrefabFactory::CreateActorFromPrefab("CameraActor");
	auto actor1 = PrefabFactory::CreateActorFromPrefab("DoomMenuPlayer");
}

void LVLDoomMainMenu::Update()
{
	Scene::Update();
}

void LVLDoomMainMenu::Render()
{
	Scene::Render();
}

void LVLDoomMainMenu::Close()
{
	Scene::Close();
}