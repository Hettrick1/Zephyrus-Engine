#include "DoomMenuPC.h"
#include "BooleanActions.h"
#include "Axis2DAction.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Temp/ChildScenes/LVLDoom.h"
#include "ComponentFactory.h"
#include "HudElement.h"
#include "Timer.h"
#include "TextRenderer.h"
#include "HudManager.h" 

DoomMenuPC::DoomMenuPC(Actor* pOwner, int pUpdateOrder)
	: Component(pOwner, "DoomMenuPC", pUpdateOrder)
{
	InputManager& inputManager = InputManager::Instance();
	inputManager.CreateNewBooleanKeyBinding(this, "Play", SDLK_RETURN);
}

DoomMenuPC::~DoomMenuPC()
{
}

void DoomMenuPC::Deserialize(const rapidjson::Value& pData)
{
	Component::Deserialize(pData);
}

void DoomMenuPC::Serialize(Serialization::Json::JsonWriter& pWriter)
{
	Component::Serialize(pWriter);
}

void DoomMenuPC::OnStart()
{
	Component::OnStart();
	Texture* damageIndicator = Assets::LoadTexture("Sprites/Doom/MainMenu.png", "MainMenu");
	mDoomMenu = new HudImage(*damageIndicator, Vector2D(0, 0), 2);
	mDoomMenu->SetTint(Vector4D(1.0, 1.0, 1.0, 1.0));

	mPressEnter = new HudText("Press Enter To Begin", Vector2D(0.0f, -800.0f), 1.0f, Vector4D(1.0, 1.0, 1.0, 0.0), TextAlignment::CENTER);

	SDL_SetRelativeMouseMode(SDL_TRUE);
}

void DoomMenuPC::OnActionStarted(InputActions* action)
{
	if (action->GetType() == ActionType::Boolean)
	{
		BooleanActions* Triggeredaction = static_cast<BooleanActions*>(action);
		if (Triggeredaction && Triggeredaction->GetName() == "Play")
		{
			SceneManager::LoadSceneWithFile("../Content/Maps/LevelDoom.ZPMap");
		}
	}
}

void DoomMenuPC::OnActionTriggered(InputActions* action)
{
}

void DoomMenuPC::OnActionEnded(InputActions* action)
{
}

void DoomMenuPC::Update()
{
	Component::Update();
	mTimer += Timer::deltaTime;
	mPressEnter->SetColor(Vector4D(mPressEnter->GetColor().xyz, (Maths::Sin(mTimer * 2) + 1) * 0.5f));
}
