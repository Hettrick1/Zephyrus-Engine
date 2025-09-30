#include "PlayerController3D.h"
#include "BooleanActions.h"
#include "Axis2DAction.h"
#include "InputManager.h"
#include "Actor.h"
#include "Log.h"
#include "Timer.h"

PlayerController3D::PlayerController3D(Actor* pOwner, int pUpdateOrder)
	: Component(pOwner, "PlayerController3D", pUpdateOrder),
	mCanGoRight(true), mCanGoLeft(true), mCanGoUp(true), mCanGoDown(true), mCanGoBackward(true), mCanGoForward(true),
	mSpeed(10), mYaw(0), mPitch(0)
{
	InputManager& inputManager = InputManager::Instance();
	inputManager.CreateNewBooleanKeyBinding(this, "up", SDLK_SPACE);
	inputManager.CreateNewBooleanKeyBinding(this, "down", SDLK_LSHIFT);
	inputManager.CreateNewBooleanKeyBinding(this, "forward", SDLK_w);
	inputManager.CreateNewBooleanKeyBinding(this, "left", SDLK_a);
	inputManager.CreateNewBooleanKeyBinding(this, "backward", SDLK_s);
	inputManager.CreateNewBooleanKeyBinding(this, "right", SDLK_d);
	inputManager.CreateNewAxis2DBinding(this, "Mouse");
}

void PlayerController3D::OnActionStarted(InputActions* pAction)
{
	
}

void PlayerController3D::OnActionTriggered(InputActions* pAction)
{
	if (pAction->GetType() == ActionType::Axis2D)
	{
		SDL_SetRelativeMouseMode(SDL_TRUE);
		SDL_SetHintWithPriority(SDL_HINT_MOUSE_RELATIVE_MODE_WARP, "1", SDL_HINT_OVERRIDE); 
		Axis2DAction* axisAction = static_cast<Axis2DAction*>(pAction);
		Vector2D axis = axisAction->GetAxis();

		float sensitivity = 0.1f;

		axis.x *= sensitivity;
		axis.y *= sensitivity;;
	}
}

void PlayerController3D::OnActionEnded(InputActions* pAction)
{
}

void PlayerController3D::Update()
{
	Component::Update();
}

void PlayerController3D::SetMovementSpeed(float pSpeed)
{
	mSpeed = pSpeed;
}
