#include "EditorControllerComponent.h"
#include "BooleanActions.h"
#include "Axis2DAction.h"
#include "InputManager.h"
#include "Log.h"
#include "CameraComponent.h"
#include "Timer.h"
#include "Actor.h"

EditorControllerComponent::EditorControllerComponent(Actor* pOwner, int pUpdateOrder)
	: Component(pOwner)
{
}

void EditorControllerComponent::OnActionStarted(InputActions* pAction)
{
	if (!mInCapture) return;
	if (pAction->GetType() == ActionType::Boolean)
	{
		auto* triggeredaction = static_cast<BooleanActions*>(pAction);
		if (!triggeredaction) return;
		if (triggeredaction->GetName() == "rightClick")
		{
			mCanMove = true;
			SDL_SetRelativeMouseMode(SDL_TRUE);
		}
	}
}
void EditorControllerComponent::OnActionTriggered(InputActions* pAction)
{
	if (!mCanMove) return;

	if (pAction->GetType() == ActionType::Boolean)
	{
		auto* triggeredaction = static_cast<BooleanActions*>(pAction);
		if (!triggeredaction) return;
		if (triggeredaction->GetName() == "forward")
		{
			auto forward = mOwner->GetTransformComponent().Forward();
			mOwner->GetTransformComponent().Translate(forward * mSpeed * Timer::deltaTime);
		}
		else if (triggeredaction->GetName() == "left")
		{
			auto right = mOwner->GetTransformComponent().Right();
			mOwner->GetTransformComponent().Translate(-right * mSpeed * Timer::deltaTime);
		}
		else if (triggeredaction->GetName() == "backward")
		{
			auto forward = mOwner->GetTransformComponent().Forward();
			mOwner->GetTransformComponent().Translate(-forward * mSpeed * Timer::deltaTime);
		}
		else if (triggeredaction->GetName() == "right")
		{
			auto right = mOwner->GetTransformComponent().Right();
			mOwner->GetTransformComponent().Translate(right * mSpeed * Timer::deltaTime);
		}
		else if (triggeredaction->GetName() == "up")
		{
			auto up = Vector3D::unitZ;
			mOwner->GetTransformComponent().Translate(up * mSpeed * Timer::deltaTime);
		}
		else if (triggeredaction->GetName() == "down")
		{
			auto up = Vector3D::unitZ;
			mOwner->GetTransformComponent().Translate(-up * mSpeed * Timer::deltaTime);
		}
	}
	else if (pAction->GetType() == ActionType::Axis2D)
	{
		Axis2DAction* axisAction = static_cast<Axis2DAction*>(pAction);
		Vector2D axis = axisAction->GetAxis();

		mYaw += axis.x * mMouseSensitivity;
		mPitch += axis.y * -mMouseSensitivity;

		if (mPitch > 89.0f)  mPitch = 89.0f;
		if (mPitch < -89.0f) mPitch = -89.0f;

		float yawRad = Maths::ToRad(mYaw);
		float pitchRad = Maths::ToRad(mPitch);

		Quaternion qYaw(Vector3D::unitZ, yawRad);
		Quaternion qPitch(Vector3D::unitX, pitchRad);

		Quaternion finalRot = Quaternion::Concatenate(qPitch, qYaw);

		mOwner->GetTransformComponent().SetRotation(finalRot);
		
	}
}
void EditorControllerComponent::OnActionEnded(InputActions* pAction)
{
	if (pAction->GetType() == ActionType::Boolean)
	{
		auto* triggeredaction = dynamic_cast<BooleanActions*>(pAction);
		if (!triggeredaction) return;
		if (triggeredaction->GetName() == "rightClick")
		{
			mCanMove = false;
			SDL_SetRelativeMouseMode(SDL_FALSE);
		}
	}
}

void EditorControllerComponent::OnStart()
{
	Component::OnStart();
	InputManager& inputManager = InputManager::Instance();
	inputManager.CreateNewBooleanKeyBinding(this, "up", SDLK_SPACE);
	inputManager.CreateNewBooleanKeyBinding(this, "down", SDLK_LSHIFT);
	inputManager.CreateNewBooleanKeyBinding(this, "forward", SDLK_w);
	inputManager.CreateNewBooleanKeyBinding(this, "left", SDLK_a);
	inputManager.CreateNewBooleanKeyBinding(this, "backward", SDLK_s);
	inputManager.CreateNewBooleanKeyBinding(this, "right", SDLK_d);
	inputManager.CreateNewBooleanKeyBinding(this, "delete", SDLK_DELETE);
	inputManager.CreateNewBooleanBtnBinding(this, "rightClick", SDL_BUTTON_RIGHT);
	inputManager.CreateNewAxis2DBinding(this, "Mouse");
	SDL_SetRelativeMouseMode(SDL_FALSE);
}

void EditorControllerComponent::Update()
{

}

void EditorControllerComponent::SetMovementSpeed(float pSpeed)
{
	mSpeed = pSpeed;
}

void EditorControllerComponent::SetIsInSceneCapture(bool pInCapture)
{
	mInCapture = pInCapture;
}
