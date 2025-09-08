#include "DoomMenuPC.h"
#include "BooleanActions.h"
#include "Axis2DAction.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "Temp/ChildScenes/LVLDoom.h"
#include "ComponentFactory.h"

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

void DoomMenuPC::OnActionStarted(InputActions* action)
{
	if (action->GetType() == ActionType::Boolean)
	{
		BooleanActions* Triggeredaction = static_cast<BooleanActions*>(action);
		if (Triggeredaction && Triggeredaction->GetName() == "Play")
		{
			SceneManager::LoadScene(new LVLDoom());
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
}
