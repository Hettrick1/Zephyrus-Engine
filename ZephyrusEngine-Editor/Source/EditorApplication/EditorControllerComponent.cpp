#include "pch.h"
#include "EditorControllerComponent.h"
#include "InputActionBool.h"
#include "InputActionAxis2D.h"
#include "InputManager.h"
#include "Log.h"
#include "Timer.h"
#include "Actor.h"

namespace Zephyrus::ActorComponent {
	EditorControllerComponent::EditorControllerComponent(Actor* pOwner, int pUpdateOrder)
		: Component(pOwner)
	{
	}

	void EditorControllerComponent::OnActionStarted(InputAction* pAction)
	{
		// if (!mInCapture) return;
		// if (pAction->GetType() == ActionType::Boolean)
		// {
		// 	auto* triggeredaction = static_cast<BooleanActions*>(pAction);
		// 	if (!triggeredaction) return;
		// 	if (triggeredaction->GetName() == "rightClick")
		// 	{
		// 		mCanMove = true;
		// 		SDL_WarpMouseGlobal(960, 540);
		// 		SDL_SetRelativeMouseMode(SDL_TRUE);
		// 	}
		// }
	}
	void EditorControllerComponent::OnActionTriggered(InputAction* pAction)
	{
		// if (!mCanMove) return;
		//
		// if (pAction->GetType() == ActionType::Boolean)
		// {
		// 	auto* triggeredaction = static_cast<BooleanActions*>(pAction);
		// 	if (!triggeredaction) return;
		// 	if (triggeredaction->GetName() == "forward")
		// 	{
		// 		auto forward = mOwner->GetTransformComponent().Forward();
		// 		mOwner->GetTransformComponent().Translate(forward * mSpeed * Timer::deltaTime);
		// 	}
		// 	else if (triggeredaction->GetName() == "left")
		// 	{
		// 		auto right = mOwner->GetTransformComponent().Right();
		// 		mOwner->GetTransformComponent().Translate(-right * mSpeed * Timer::deltaTime);
		// 	}
		// 	else if (triggeredaction->GetName() == "backward")
		// 	{
		// 		auto forward = mOwner->GetTransformComponent().Forward();
		// 		mOwner->GetTransformComponent().Translate(-forward * mSpeed * Timer::deltaTime);
		// 	}
		// 	else if (triggeredaction->GetName() == "right")
		// 	{
		// 		auto right = mOwner->GetTransformComponent().Right();
		// 		mOwner->GetTransformComponent().Translate(right * mSpeed * Timer::deltaTime);
		// 	}
		// 	else if (triggeredaction->GetName() == "up")
		// 	{
		// 		auto up = Vector3D::unitZ;
		// 		mOwner->GetTransformComponent().Translate(up * mSpeed * Timer::deltaTime);
		// 	}
		// 	else if (triggeredaction->GetName() == "down")
		// 	{
		// 		auto up = Vector3D::unitZ;
		// 		mOwner->GetTransformComponent().Translate(-up * mSpeed * Timer::deltaTime);
		// 	}
		// }
		// else if (pAction->GetType() == ActionType::Axis2D)
		// {
		// 	Axis2DAction* axisAction = static_cast<Axis2DAction*>(pAction);
		// 	Vector2D axis = axisAction->GetAxis();
		// 	
		// 	mYaw += axis.x * mMouseSensitivity;
		// 	mPitch += axis.y * -mMouseSensitivity;
		// 	
		// 	if (mPitch > 89.0f)  mPitch = 89.0f;
		// 	if (mPitch < -89.0f) mPitch = -89.0f;
		// 	
		// 	float yawRad = zpMaths::ToRad(mYaw);
		// 	float pitchRad = zpMaths::ToRad(mPitch);
		// 	
		// 	Quaternion qYaw(Vector3D::unitZ, yawRad);
		// 	Quaternion qPitch(Vector3D::unitX, pitchRad);
		// 	
		// 	Quaternion finalRot = Quaternion::Concatenate(qPitch, qYaw);
		// 	
		// 	mOwner->GetTransformComponent().SetRotation(finalRot);
		// }
	}
	void EditorControllerComponent::OnActionEnded(InputAction* pAction)
	{
		// if (pAction->GetType() == ActionType::Boolean)
		// {
		// 	auto* triggeredaction = dynamic_cast<BooleanActions*>(pAction);
		// 	if (!triggeredaction) return;
		// 	if (triggeredaction->GetName() == "rightClick")
		// 	{
		// 		mCanMove = false;
		// 		SDL_SetRelativeMouseMode(SDL_FALSE);
		// 	}
		// }
	}

	void EditorControllerComponent::OnStart()
	{
		Component::OnStart();
		// InputManager& inputManager = InputManager::Instance();
		// inputManager.CreateNewBooleanKeyBinding(this, "up", SDLK_SPACE);
		// inputManager.CreateNewBooleanKeyBinding(this, "down", SDLK_LSHIFT);
		// inputManager.CreateNewBooleanKeyBinding(this, "forward", SDLK_w);
		// inputManager.CreateNewBooleanKeyBinding(this, "left", SDLK_a);
		// inputManager.CreateNewBooleanKeyBinding(this, "backward", SDLK_s);
		// inputManager.CreateNewBooleanKeyBinding(this, "right", SDLK_d);
		// inputManager.CreateNewBooleanKeyBinding(this, "delete", SDLK_DELETE);
		// inputManager.CreateNewBooleanBtnBinding(this, "rightClick", SDL_BUTTON_RIGHT);
		// inputManager.CreateNewAxis2DBinding(this, "Mouse");
		// SDL_SetRelativeMouseMode(SDL_FALSE);

		if (mEditorInputManager)
		{
			auto& camera = mEditorInputManager->CreateAxis2D("Camera");
			camera.IsMouseAxis = true;
			camera.OnTriggered = [this](Vector2D delta){ Rotate(delta); };

			auto& click = mEditorInputManager->CreateBool("Click");
			click.BindMouseButton(GLFW_MOUSE_BUTTON_2);
			click.OnStarted = [this]()
			{
				mCanMove = true;
				mEditorInputManager->SetCursorRelative(true);
			};
			click.OnReleased = [this]()
			{
				mCanMove = false;
				mEditorInputManager->SetCursorRelative(false);
			};
			
			auto& move = mEditorInputManager->CreateAxis2D("Move");
			move.BindKeyValue(GLFW_KEY_W, Vector2D(1.0f, 0.0f));
			move.BindKeyValue(GLFW_KEY_S, Vector2D(-1.0f, 0.0f));
			move.BindKeyValue(GLFW_KEY_A, Vector2D(0.0f, -1.0f));
			move.BindKeyValue(GLFW_KEY_D, Vector2D(0.0f, 1.0f));
			move.OnTriggered = [this](Vector2D delta){ Move(delta); };
			
			auto& upDown = mEditorInputManager->CreateAxis1D("UpDown");
			upDown.OnTriggered = [this](float delta){ UpDown(delta); };
			upDown.BindKeyValue(GLFW_KEY_SPACE, 1.0f);
			upDown.BindKeyValue(GLFW_KEY_LEFT_SHIFT, -1.0f);
		}
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

	void EditorControllerComponent::SetInputManager(InputManager* pInputManager)
	{
		mEditorInputManager = pInputManager;
	}

	void EditorControllerComponent::Rotate(Vector2D delta)
	{
		if (mCanMove)
		{
			mYaw += delta.x * mMouseSensitivity;
			mPitch += delta.y * -mMouseSensitivity;
			
			if (mPitch > 89.0f)  mPitch = 89.0f;
			if (mPitch < -89.0f) mPitch = -89.0f;
			
			float yawRad = zpMaths::ToRad(mYaw);
			float pitchRad = zpMaths::ToRad(mPitch);
			
			Quaternion qYaw(Vector3D::unitZ, yawRad);
			Quaternion qPitch(Vector3D::unitX, pitchRad);
			
			Quaternion finalRot = Quaternion::Concatenate(qPitch, qYaw);
			
			mOwner->GetTransformComponent().SetRotation(finalRot);
		}
	}

	void EditorControllerComponent::Move(Vector2D delta)
	{
		if (mCanMove)
		{
			if (delta.x != 0)
			{
				auto forward = mOwner->GetTransformComponent().Forward();
				mOwner->GetTransformComponent().Translate(forward * delta.x * mSpeed * Timer::deltaTime);
			}
			if (delta.y != 0)
			{
				auto right = mOwner->GetTransformComponent().Right();
				mOwner->GetTransformComponent().Translate(right * delta.y * mSpeed * Timer::deltaTime);
			}
		}
	}

	void EditorControllerComponent::UpDown(float direction)
	{
		auto up = Vector3D::unitZ;
		mOwner->GetTransformComponent().Translate(up * direction * mSpeed * Timer::deltaTime);
	}
}
