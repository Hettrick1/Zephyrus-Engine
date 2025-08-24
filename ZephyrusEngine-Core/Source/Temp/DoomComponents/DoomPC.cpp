#include "DoomPC.h"

#include "BooleanActions.h"
#include "Axis2DAction.h"
#include "InputManager.h"
#include "Actor.h"
#include "Log.h"
#include "Timer.h"
#include "CameraComponent.h"
#include "FlipbookComponent.h"
#include "Scene.h"
#include "SceneManager.h"
#include "Physics/PhysicManager.h"
#include "DoomPlayerComponent.h"

DoomPC::DoomPC(Actor* pOwner, int pUpdateOrder)
	: Component(pOwner, pUpdateOrder), playerRbRef(nullptr), goRight(true), goLeft(true), goForward(true), goBackward(true)
{
	InputManager& inputManager = InputManager::Instance();
	inputManager.CreateNewAxis2DBinding(this, "Movement", SDLK_d, SDLK_a, SDLK_w, SDLK_s);
	inputManager.CreateNewBooleanBtnBinding(this, "Shoot", SDL_BUTTON_LEFT);
	inputManager.CreateNewBooleanKeyBinding(this, "ChangeWeapon", SDLK_TAB);
	inputManager.CreateNewBooleanKeyBinding(this, "WireFrame", SDLK_F1);
	inputManager.CreateNewAxis2DBinding(this, "Mouse");
	if (playerRbRef == nullptr)
	{
		playerRbRef = mOwner->GetRigidBody();
	}
}

DoomPC::~DoomPC()
{
	if (playerRbRef != nullptr)
	{
		playerRbRef = nullptr;
	}
}

void DoomPC::Deserialize(const rapidjson::Value& pData)
{
	Component::Deserialize(pData);
}

void DoomPC::OnActionStarted(InputActions* action)
{
	if (action->GetType() == ActionType::Boolean)
	{
		BooleanActions* Triggeredaction = static_cast<BooleanActions*>(action);
		DoomPlayerComponent* playerComponentRef = mOwner->GetComponentOfType<DoomPlayerComponent>();
		if (Triggeredaction && Triggeredaction->GetName() == "Shoot")
		{
			FlipbookComponent* fb = mOwner->GetComponentOfType<FlipbookComponent>();
			int amoNeeded = 0;
			if (playerComponentRef->GetWeapon() == Weapons::Gun)
			{
				amoNeeded = 1;
			}
			else
			{
				amoNeeded = 2;
			}
			if (playerComponentRef->GetAmo() >= amoNeeded && fb->IsAnimationEnded())
			{
				fb->PlayAnimation();
				playerComponentRef->Shoot(amoNeeded);
			}
		}
		if (Triggeredaction && Triggeredaction->GetName() == "ChangeWeapon")
		{
			playerComponentRef->ChangeWeapon();
		}
		if (Triggeredaction && Triggeredaction->GetName() == "WireFrame")
		{
			if (SceneManager::ActiveScene->GetRenderer()->GetWireFrame())
			{
				SceneManager::ActiveScene->GetRenderer()->SetWireFrameMode(false);
			}
			else
			{
				SceneManager::ActiveScene->GetRenderer()->SetWireFrameMode(true);
			}
		}
	}
}

void DoomPC::OnActionTriggered(InputActions* action)
{
	if (action->GetType() == ActionType::Axis2D)
	{
		Axis2DAction* Triggeredaction = static_cast<Axis2DAction*>(action);
		if (Triggeredaction && Triggeredaction->GetName() == "Mouse")
		{
			SDL_SetRelativeMouseMode(SDL_TRUE);
			SDL_SetHintWithPriority(SDL_HINT_MOUSE_RELATIVE_MODE_WARP, "1", SDL_HINT_OVERRIDE);
			Vector2D axis = Triggeredaction->GetAxis();

			float sensitivity = 0.1f;
			axis.x *= sensitivity;
			mOwner->GetTransformComponent().RotateZ(axis.x);
		}
		if (Triggeredaction && Triggeredaction->GetName() == "Movement")
		{
			Vector3D start = mOwner->GetTransformComponent().GetPosition();
			Vector3D end;
			HitResult hit;

			Vector2D axis = Triggeredaction->GetAxis();

			Vector3D forward = mOwner->GetTransformComponent().GetWorldTransform().GetYAxis();
			Vector3D right = mOwner->GetComponentOfType<CameraComponent>()->GetWorldTransform().GetXAxis();
			Vector3D moveDirection = forward * axis.y + right * -axis.x;


			moveDirection.Normalize();
			moveDirection *= 4;
			playerRbRef->SetVelocity(moveDirection);

			end = start + Vector3D(0, 0.5, 0);
			PhysicManager::Instance().LineTrace(start, end, hit, mOwner);
			DebugLine* line = new DebugLine(start, end, hit);
			SceneManager::ActiveScene->GetRenderer()->AddDebugLine(line); 

			if (hit.HitActor != nullptr && hit.HitActor->HasTag("Wall") && playerRbRef->GetVelocity().y > 0)
			{
				playerRbRef->SetVelocity(Vector3D(playerRbRef->GetVelocity().x, 0, playerRbRef->GetVelocity().z));
			}

			end = start + Vector3D(0, -0.5, 0);
			hit.Reset();
			PhysicManager::Instance().LineTrace(start, end, hit, mOwner);
			DebugLine* line3 = new DebugLine(start, end, hit);
			SceneManager::ActiveScene->GetRenderer()->AddDebugLine(line3);

			if (hit.HitActor != nullptr && hit.HitActor->HasTag("Wall") && playerRbRef->GetVelocity().y < 0)
			{
				playerRbRef->SetVelocity(Vector3D(playerRbRef->GetVelocity().x, 0, playerRbRef->GetVelocity().z));
			}

			end = start + Vector3D(0.5, 0, 0);
			hit.Reset();
			PhysicManager::Instance().LineTrace(start, end, hit, mOwner);
			DebugLine* line1 = new DebugLine(start, end, hit);
			SceneManager::ActiveScene->GetRenderer()->AddDebugLine(line1); 

			if (hit.HitActor != nullptr && hit.HitActor->HasTag("Wall") && playerRbRef->GetVelocity().x > 0)
			{
				playerRbRef->SetVelocity(Vector3D(0, playerRbRef->GetVelocity().y, playerRbRef->GetVelocity().z));
			}

			end = start + Vector3D(-0.5, 0, 0);
			hit.Reset();
			PhysicManager::Instance().LineTrace(start, end, hit, mOwner);
			DebugLine* line2 = new DebugLine(start, end, hit);
			SceneManager::ActiveScene->GetRenderer()->AddDebugLine(line2);

			if (hit.HitActor != nullptr && hit.HitActor->HasTag("Wall") && playerRbRef->GetVelocity().x < 0)
			{
				playerRbRef->SetVelocity(Vector3D(0, playerRbRef->GetVelocity().y, playerRbRef->GetVelocity().z));
			}
		}
	}
}

void DoomPC::OnActionEnded(InputActions* action)
{
}

void DoomPC::Update()
{
	if (playerRbRef == nullptr)
	{
		playerRbRef = mOwner->GetRigidBody();
		ZP_CORE_ERROR("Rigidboy not set in the player controller");
	}
}