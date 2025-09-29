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
#include "DoomPlayerComponent.h"

DoomPC::DoomPC(Actor* pOwner, int pUpdateOrder)
	: Component(pOwner, "DoomPC", pUpdateOrder), goRight(true), goLeft(true), goForward(true), goBackward(true)
{
	InputManager& inputManager = InputManager::Instance();
	inputManager.CreateNewAxis2DBinding(this, "Movement", SDLK_d, SDLK_a, SDLK_w, SDLK_s);
	inputManager.CreateNewBooleanBtnBinding(this, "Shoot", SDL_BUTTON_LEFT);
	inputManager.CreateNewBooleanKeyBinding(this, "ChangeWeapon", SDLK_TAB);
	inputManager.CreateNewAxis2DBinding(this, "Mouse");
}

DoomPC::~DoomPC()
{

}

void DoomPC::Deserialize(const rapidjson::Value& pData)
{
	Component::Deserialize(pData);
}

void DoomPC::Serialize(Serialization::Json::JsonWriter& pWriter)
{
	Component::Serialize(pWriter);
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
	}
}

void DoomPC::OnActionTriggered(InputActions* action)
{
	if (action->GetType() == ActionType::Axis2D)
	{
		NewCameraComponent* cam = mOwner->GetComponentOfType<NewCameraComponent>();
		Axis2DAction* Triggeredaction = static_cast<Axis2DAction*>(action);
		DoomPlayerComponent* playerComponentRef = mOwner->GetComponentOfType<DoomPlayerComponent>();
		if (Triggeredaction && Triggeredaction->GetName() == "Mouse")
		{
			SDL_SetRelativeMouseMode(SDL_TRUE);
			SDL_SetHintWithPriority(SDL_HINT_MOUSE_RELATIVE_MODE_WARP, "1", SDL_HINT_OVERRIDE);

			Vector2D axis = Triggeredaction->GetAxis();

			float sensitivity = 0.1f;
			axis.x *= sensitivity;
			cam->RelativeRotateZ(axis.x);
		}
		if (Triggeredaction && Triggeredaction->GetName() == "Movement")
		{
			Vector2D axis = Triggeredaction->GetAxis();

			Vector3D forward = cam->GetWorldTransform().GetYAxis();
			Vector3D right = cam->GetWorldTransform().GetXAxis();
			Vector3D moveDir = forward * axis.y + right * -axis.x;

			if (moveDir.LengthSq() > 0.0f)
			{
				moveDir.Normalize();

				float moveForce = 100.0f;
				Vector3D forceToApply = moveDir * moveForce;

				auto rb = mOwner->GetRigidBody();
				if (rb && rb->GetRigidBody())
				{
					btVector3 bulletForce(forceToApply.x, forceToApply.y, 0.0f);
					rb->GetRigidBody()->applyCentralForce(bulletForce);
					rb->GetRigidBody()->activate(true);
					btVector3 vel = rb->GetRigidBody()->getLinearVelocity();
					btVector3 horizontalVel(vel.x(), vel.y(), 0);
					float maxSpeed = 2.0f;

					if (horizontalVel.length() > maxSpeed)
					{
						horizontalVel = horizontalVel.normalized() * maxSpeed;
						vel.setX(horizontalVel.x());
						vel.setY(horizontalVel.y());
						rb->GetRigidBody()->setLinearVelocity(vel);
					}
				}
			}
		}
	}
}

void DoomPC::OnActionEnded(InputActions* action)
{
}

void DoomPC::Update()
{
}