#include "BowlingPC.h"
#include "BooleanActions.h"
#include "Axis2DAction.h"
#include "InputManager.h"
#include "Actor.h"
#include "Log.h"
#include "CameraComponent.h"
#include "Timer.h"
#include "CameraManager.h"
#include "BowlingPlayer.h"

BowlingPC::BowlingPC(Actor* pOwner, int pUpdateOrder)
	: ActorMovementComponent(pOwner, pUpdateOrder),
	mCanGoRight(true), mCanGoLeft(true), mCanGoUp(true), mCanGoDown(true), mCanGoBackward(true), mCanGoForward(true),
	mSpeed(10), mYaw(0), mPitch(0), mShootStrengh(0), mXPosition(0)
{
	InputManager& inputManager = InputManager::Instance();
	inputManager.CreateNewBooleanKeyBinding(this, "shoot", SDLK_SPACE);
	inputManager.CreateNewBooleanKeyBinding(this, "tab", SDLK_TAB);
	inputManager.CreateNewBooleanKeyBinding(this, "d", SDLK_d);
	inputManager.CreateNewBooleanKeyBinding(this, "a", SDLK_a);
	inputManager.CreateNewAxis2DBinding(this, "Mouse");
	mPlayer = dynamic_cast<BowlingPlayer*>(mOwner);
}

void BowlingPC::OnActionStarted(InputActions* action)
{
	if (action->GetType() == ActionType::Boolean)
	{
		BooleanActions* Triggeredaction = static_cast<BooleanActions*>(action);
		if (Triggeredaction && Triggeredaction->GetName() == "tab")
		{
			mCameraIndex = mCameraIndex == 0 ? 1 : 0;
			CameraManager::Instance().SetCurrentCamera(mCameraIndex);
		}
	}
}

void BowlingPC::OnActionTriggered(InputActions* action)
{
	if (action->GetType() == ActionType::Boolean)
	{
		BooleanActions* Triggeredaction = static_cast<BooleanActions*>(action);
		if (Triggeredaction && Triggeredaction->GetName() == "shoot")
		{
			if (mShootStrengh < 1)
			{
				mShootStrengh += Timer::deltaTime;
			}
			else if (mShootStrengh >= 1) 
			{
				mShootStrengh = 1;
			}
			mPlayer->IncrementIndicatorSize(mShootStrengh);
		}
		if (Triggeredaction && Triggeredaction->GetName() == "d")
		{
			mXPosition -= 2 * Timer::deltaTime;
		}
		if (Triggeredaction && Triggeredaction->GetName() == "a")
		{
			mXPosition += 2 * Timer::deltaTime;
		}
		mPlayer->MoveSpawnPointX(mXPosition);
	}
	else if (action->GetType() == ActionType::Axis2D)
	{
		SDL_SetRelativeMouseMode(SDL_TRUE);
		SDL_SetHintWithPriority(SDL_HINT_MOUSE_RELATIVE_MODE_WARP, "1", SDL_HINT_OVERRIDE);
		Axis2DAction* axisAction = static_cast<Axis2DAction*>(action);
		Vector2D axis = axisAction->GetAxis();

		float sensitivity = 0.01f;

		axis.x *= sensitivity;
		mPitch += axis.x;
		if (mPitch > -10 && mPitch < 10)
		{
			mPlayer->RotateSpawnPointZ(axis.x);
		}
		if (mPitch <= -10)
		{
			mPitch = -10;
		}
		if (mPitch >= 10)
		{
			mPitch = 10;
		}
	}
}

void BowlingPC::OnActionEnded(InputActions* action)
{
	if (action->GetType() == ActionType::Boolean)
	{
		BooleanActions* Triggeredaction = static_cast<BooleanActions*>(action);
		if (Triggeredaction && Triggeredaction->GetName() == "shoot")
		{
			
			mPlayer->ThrowBowlingBall(mShootStrengh);
			mShootStrengh = 0; 
			mPlayer->IncrementIndicatorSize(mShootStrengh);
		}
	}
}

void BowlingPC::Update()
{
	ActorMovementComponent::Update();
}

void BowlingPC::EnableDirection(Vector3D pDirection)
{
}

void BowlingPC::DisableDirection(Vector3D pDirection)
{
}

void BowlingPC::SetMovementSpeed(float pSpeed)
{
	mSpeed = pSpeed;
}
