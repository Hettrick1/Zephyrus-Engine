#include "PlayerController3D.h"
#include "BooleanActions.h"
#include "Axis2DAction.h"
#include "InputManager.h"
#include "Actor.h"
#include "Log.h"
#include "CameraComponent.h"
#include "Timer.h"

PlayerController3D::PlayerController3D(Actor* pOwner, int pUpdateOrder)
	: ActorMovementComponent(pOwner, pUpdateOrder),
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

void PlayerController3D::OnActionStarted(InputActions* action)
{
	
}

void PlayerController3D::OnActionTriggered(InputActions* action)
{
	if (action->GetType() == ActionType::Boolean)
	{
		auto* Triggeredaction = dynamic_cast<BooleanActions*>(action);
		if (Triggeredaction && Triggeredaction->GetName() == "forward")
		{
			if (mCanGoUp)
			{
				SetSpeedZ(mSpeed);
			}
			else {
				SetSpeedZ(0);
			}
		}
		else if (Triggeredaction && Triggeredaction->GetName() == "left")
		{
			if (mCanGoLeft)
			{
				SetSpeedX(-mSpeed);
			}
			else {
				SetSpeedX(0);
			}
		}
		else if (Triggeredaction && Triggeredaction->GetName() == "backward")
		{
			if (mCanGoDown)
			{
				SetSpeedZ(-mSpeed);
			}
			else {
				SetSpeedZ(0);
			}
		}
		else if (Triggeredaction && Triggeredaction->GetName() == "right")
		{
			if (mCanGoRight)
			{
				SetSpeedX(mSpeed);
			}
			else {
				SetSpeedX(0);
			}
		}
		else if (Triggeredaction && Triggeredaction->GetName() == "up")
		{
			if (mCanGoUp)
			{
				SetSpeedY(mSpeed);
			}
			else
			{
				SetSpeedY(0);
			}
		}
		else if (Triggeredaction && Triggeredaction->GetName() == "down")
		{
			if (mCanGoDown)
			{
				SetSpeedY(-mSpeed);
			}
			else
			{
				SetSpeedY(0);
			}
		}
	}
	else if (action->GetType() == ActionType::Axis2D)
	{
		SDL_SetRelativeMouseMode(SDL_TRUE);
		SDL_SetHintWithPriority(SDL_HINT_MOUSE_RELATIVE_MODE_WARP, "1", SDL_HINT_OVERRIDE); 
		Axis2DAction* axisAction = static_cast<Axis2DAction*>(action);
		Vector2D axis = axisAction->GetAxis();

		float sensitivity = 0.1f;

		axis.x *= sensitivity;
		axis.y *= sensitivity;

		//mOwner->GetTransformComponent().RotateZ(axis.x);
		//mOwner->GetTransformComponent().RotateX(-axis.y);

		//mOwner->GetTransformComponent().GetRotation().Normalize(); 

		//Log::Info(std::to_string(axis.x));
	}
}

void PlayerController3D::OnActionEnded(InputActions* action)
{
	if (action->GetType() == ActionType::Boolean) { 
		auto* Triggeredaction = dynamic_cast<BooleanActions*>(action); 
		if (Triggeredaction && Triggeredaction->GetName() == "up") 
		{
			SetSpeedY(0);
		}
		else if (Triggeredaction && Triggeredaction->GetName() == "left")
		{
			SetSpeedX(0);
		}
		else if (Triggeredaction && Triggeredaction->GetName() == "down")
		{
			SetSpeedY(0); 
		}
		else if (Triggeredaction && Triggeredaction->GetName() == "right")
		{
			SetSpeedX(0);
		}
		else if (Triggeredaction && Triggeredaction->GetName() == "forward")
		{
			SetSpeedZ(0);
		}
		else if (Triggeredaction && Triggeredaction->GetName() == "backward")
		{
			SetSpeedZ(0);
		}
	}
}

void PlayerController3D::Update()
{
	ActorMovementComponent::Update();
}

void PlayerController3D::EnableDirection(Vector3D pDirection)
{
}

void PlayerController3D::DisableDirection(Vector3D pDirection)
{
}

void PlayerController3D::SetMovementSpeed(float pSpeed)
{
	mSpeed = pSpeed;
}
