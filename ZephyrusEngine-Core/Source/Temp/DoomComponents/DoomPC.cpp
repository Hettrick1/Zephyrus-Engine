#include "pch.h"
#include "DoomPC.h"

#include "InputActionBool.h"
#include "InputActionAxis2D.h"
#include "InputManager.h"
#include "Actor.h"
#include "Log.h"
#include "Timer.h"
#include "FlipbookComponent.h"
#include "Scene.h"
#include "SceneManager.h"
#include "DoomPlayerComponent.h"

namespace Zephyrus::ActorComponent
{
	DoomPC::DoomPC(Actor* pOwner, int pUpdateOrder)
		: Component(pOwner, "DoomPC", pUpdateOrder), goRight(true), goLeft(true), goForward(true), goBackward(true)
	{

		auto& mouseAxis = mOwner->GetSceneContext()->GetInputManager()->CreateAxis2D("Mouse");
		mouseAxis.IsMouseAxis = true;
		mouseAxis.OnTriggered = [this](Vector2D delta){ Rotate(delta); };
		mOwner->GetSceneContext()->GetInputManager()->SetCursorRelative(true);

		auto& moveAxis = mOwner->GetSceneContext()->GetInputManager()->CreateAxis2D("Movement");
		moveAxis.BindKeyValue(GLFW_KEY_W, {0.0f, 1.0f});
		moveAxis.BindKeyValue(GLFW_KEY_S, {0.0f, -1.0f});
		moveAxis.BindKeyValue(GLFW_KEY_A, {-1.0f, 0.0f});
		moveAxis.BindKeyValue(GLFW_KEY_D, {1.0f, 0.0f});
		moveAxis.OnTriggered = [this](Vector2D delta){ Move(delta); };
	}

	DoomPC::~DoomPC()
	{

	}

	void DoomPC::Deserialize(Serialization::IDeserializer& pReader)
	{
		Component::Deserialize(pReader);
	}

	void DoomPC::Serialize(Serialization::ISerializer& pWriter)
	{
		Component::Serialize(pWriter);
	}

	void DoomPC::Move(Vector2D axis)
	{
		// REWORK MOVEMENTS
		CameraComponent* cam = mOwner->GetComponentOfType<CameraComponent>();
		Vector3D forward = cam->GetWorldTransform().GetYAxis();
		Vector3D right = cam->GetWorldTransform().GetXAxis();
		Vector3D moveDir = forward * axis.y + right * -axis.x;
		
		if (moveDir.LengthSq() > 0.0f)
		{
			moveDir.Normalize();
		
			float moveForce = 1000.0f;
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

	void DoomPC::Rotate(Vector2D axis)
	{
		CameraComponent* cam = mOwner->GetComponentOfType<CameraComponent>();
		
		float sensitivity = 5.0f;
		axis.x *= sensitivity * Timer::deltaTime;
		cam->RelativeRotateZ(axis.x);
	}

	void DoomPC::Shoot()
	{
	}

	void DoomPC::OnActionStarted(InputAction* action)
	{
		// if (action->GetType() == ActionType::Boolean)
		// {
		// 	BooleanActions* Triggeredaction = static_cast<BooleanActions*>(action);
		// 	DoomPlayerComponent* playerComponentRef = mOwner->GetComponentOfType<DoomPlayerComponent>();
		// 	if (Triggeredaction && Triggeredaction->GetName() == "Shoot")
		// 	{
		// 		FlipbookComponent* fb = mOwner->GetComponentOfType<FlipbookComponent>();
		// 		int amoNeeded = 0;
		// 		if (playerComponentRef->GetWeapon() == Weapons::Gun)
		// 		{
		// 			amoNeeded = 1;
		// 		}
		// 		else
		// 		{
		// 			amoNeeded = 2;
		// 		}
		// 		if (playerComponentRef->GetAmo() >= amoNeeded && fb->IsAnimationEnded())
		// 		{
		// 			fb->PlayAnimation();
		// 			playerComponentRef->Shoot(amoNeeded);
		// 		}
		// 	}
		// 	if (Triggeredaction && Triggeredaction->GetName() == "ChangeWeapon")
		// 	{
		// 		playerComponentRef->ChangeWeapon();
		// 	}
		// }
	}

	void DoomPC::OnActionTriggered(InputAction* action)
	{
		// if (action->GetType() == ActionType::Axis2D)
		// {
		// 	CameraComponent* cam = mOwner->GetComponentOfType<CameraComponent>();
		// 	Axis2DAction* Triggeredaction = static_cast<Axis2DAction*>(action);
		// 	DoomPlayerComponent* playerComponentRef = mOwner->GetComponentOfType<DoomPlayerComponent>();
		// 	if (Triggeredaction && Triggeredaction->GetName() == "Mouse")
		// 	{
		// 		SDL_SetRelativeMouseMode(SDL_TRUE);
		// 		SDL_SetHintWithPriority(SDL_HINT_MOUSE_RELATIVE_MODE_WARP, "1", SDL_HINT_OVERRIDE);
		// 		
		// 		Vector2D axis = Triggeredaction->GetAxis();
		// 		
		// 		float sensitivity = 0.1f;
		// 		axis.x *= sensitivity;
		// 		cam->RelativeRotateZ(axis.x);
		// 	}
		// 	if (Triggeredaction && Triggeredaction->GetName() == "Movement")
		// 	{
		// 		Vector2D axis = Triggeredaction->GetAxis();
		// 		
		// 		Vector3D forward = cam->GetWorldTransform().GetYAxis();
		// 		Vector3D right = cam->GetWorldTransform().GetXAxis();
		// 		Vector3D moveDir = forward * axis.y + right * -axis.x;
		// 		
		// 		if (moveDir.LengthSq() > 0.0f)
		// 		{
		// 			moveDir.Normalize();
		// 		
		// 			float moveForce = 100.0f;
		// 			Vector3D forceToApply = moveDir * moveForce;
		// 		
		// 			auto rb = mOwner->GetRigidBody();
		// 			if (rb && rb->GetRigidBody())
		// 			{
		// 				btVector3 bulletForce(forceToApply.x, forceToApply.y, 0.0f);
		// 				rb->GetRigidBody()->applyCentralForce(bulletForce);
		// 				rb->GetRigidBody()->activate(true);
		// 				btVector3 vel = rb->GetRigidBody()->getLinearVelocity();
		// 				btVector3 horizontalVel(vel.x(), vel.y(), 0);
		// 				float maxSpeed = 2.0f;
		// 		
		// 				if (horizontalVel.length() > maxSpeed)
		// 				{
		// 					horizontalVel = horizontalVel.normalized() * maxSpeed;
		// 					vel.setX(horizontalVel.x());
		// 					vel.setY(horizontalVel.y());
		// 					rb->GetRigidBody()->setLinearVelocity(vel);
		// 				}
		// 			}
		// 		}
		// 	}
		// }
	}

	void DoomPC::OnActionEnded(InputAction* action)
	{
	}

	void DoomPC::Update()
	{
	}
}