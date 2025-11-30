#include "pch.h"
#include "DoomMenuPC.h"
#include "BooleanActions.h"
#include "Axis2DAction.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "ComponentFactory.h"
#include "HudElement.h"
#include "Timer.h"
#include "TextRenderer.h"
#include "HudManager.h" 

namespace Zephyrus::ActorComponent
{
	DoomMenuPC::DoomMenuPC(Actor* pOwner, int pUpdateOrder)
		: Component(pOwner, "DoomMenuPC", pUpdateOrder)
	{
		// InputManager& inputManager = InputManager::Instance();
		// inputManager.CreateNewBooleanKeyBinding(this, "Play", SDLK_RETURN);
	}

	DoomMenuPC::~DoomMenuPC()
	{
	}

	void DoomMenuPC::Deserialize(Serialization::IDeserializer& pReader)
	{
		Component::Deserialize(pReader);
	}

	void DoomMenuPC::Serialize(Serialization::ISerializer& pWriter)
	{
		Component::Serialize(pWriter);
	}

	void DoomMenuPC::OnStart()
	{
		Component::OnStart();
		Assets::ITexture2D* damageIndicator = Zephyrus::Assets::AssetsManager::LoadTexture("Sprites/Doom/MainMenu.png", "MainMenu");
		mDoomMenu = new Zephyrus::UI::HudImage(mOwner->GetSceneContext(), damageIndicator, Vector2D(0, 0), 2);
		mDoomMenu->SetTint(Vector4D(1.0, 1.0, 1.0, 1.0));

		mPressEnter = new Zephyrus::UI::HudText(mOwner->GetSceneContext(), "Press Enter To Begin", Vector2D(0.0f, -800.0f), 1.0f, Vector4D(1.0, 1.0, 1.0, 0.0), Zephyrus::UI::TextAlignment::CENTER);

		//SDL_SetRelativeMouseMode(SDL_TRUE);
	}

	void DoomMenuPC::OnActionStarted(InputActions* action)
	{
		if (action->GetType() == ActionType::Boolean)
		{
			BooleanActions* Triggeredaction = static_cast<BooleanActions*>(action);
			if (Triggeredaction && Triggeredaction->GetName() == "Play")
			{
				mOwner->GetSceneContext()->LoadSceneWithFile("../Content/Maps/LevelDoom.zpmap");
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
		mPressEnter->SetColor(Vector4D(mPressEnter->GetColor().xyz, (zpMaths::Sin(mTimer * 2) + 1) * 0.5f));
	}
}
