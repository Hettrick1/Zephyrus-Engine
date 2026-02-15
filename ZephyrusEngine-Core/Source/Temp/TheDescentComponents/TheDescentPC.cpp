#include "pch.h"
#include "TheDescentPC.h"

#include "InputActionBool.h"
#include "InputActionAxis2D.h"
#include "InputManager.h"
#include "Log.h"
#include "Timer.h"
#include "Actor.h"
#include "CameraComponent.h"

namespace Zephyrus::ActorComponent {
	TheDescentPC::TheDescentPC(Actor* pOwner, int pUpdateOrder)
		: Component(pOwner, "TheDescentPC", pUpdateOrder)
	{
	}

	TheDescentPC::~TheDescentPC()
	{
		mInputManager = nullptr;
	}

	void TheDescentPC::Deserialize(Serialization::IDeserializer& pReader)
	{
		Component::Deserialize(pReader);
	}

	void TheDescentPC::Serialize(Serialization::ISerializer& pWriter)
	{
		Component::Serialize(pWriter);
	}

	void TheDescentPC::OnStart()
	{
		Component::OnStart();
		mInputManager = mOwner->GetSceneContext()->GetInputManager();
		if (mInputManager)
		{
			mInputManager->SetCursorRelative(true);
			auto& camera = mInputManager->CreateAxis2D("Camera");
			camera.IsMouseAxis = true;
			camera.OnTriggered = [this](Vector2D delta){ Rotate(delta); };

			auto& click = mInputManager->CreateBool("Click");
			click.BindMouseButton(GLFW_MOUSE_BUTTON_2);
			click.OnStarted = [this]()
			{

			};
			click.OnReleased = [this]()
			{

			};
			
			auto& move = mInputManager->CreateAxis2D("Move");
			move.BindKeyValue(GLFW_KEY_W, Vector2D(0.0f, 1.0f));
			move.BindKeyValue(GLFW_KEY_S, Vector2D(0.0f, -1.0f));
			move.BindKeyValue(GLFW_KEY_A, Vector2D(-1.0f, 0.0f));
			move.BindKeyValue(GLFW_KEY_D, Vector2D(1.0f, 0.0f));
			move.OnTriggered = [this](Vector2D delta){ Move(delta); };
			
			auto& upDown = mInputManager->CreateAxis1D("UpDown");
			upDown.OnTriggered = [this](float delta){ UpDown(delta); };
			upDown.BindKeyValue(GLFW_KEY_SPACE, 1.0f);
			upDown.BindKeyValue(GLFW_KEY_LEFT_SHIFT, -1.0f);

			auto& tilt = mInputManager->CreateAxis1D("Tilt");
			tilt.OnTriggered = [this](float delta){ Tilt(delta); };
			tilt.BindKeyValue(GLFW_KEY_E, -1.0f);
			tilt.BindKeyValue(GLFW_KEY_Q, 1.0f);
		}
	}

	void TheDescentPC::Update()
	{
		Component::Update();
		float yawRad = zpMaths::ToRad(mYaw);
		float pitchRad = zpMaths::ToRad(mPitch);

		Quaternion actualRotation = mOwner->GetTransformComponent().GetRotation();
		
		Quaternion qYaw(Vector3D::unitZ, yawRad);
		Quaternion qPitch(-mOwner->GetTransformComponent().Right(), pitchRad);

		Quaternion finalRot = qYaw * qPitch * actualRotation;

		// finalRot.Normalize();
		// mOwner->GetTransformComponent().SetRotation(finalRot);
		//
		// mOwner->RotateY(mRoll);

		auto rb = mOwner->GetRigidBody();
		if (rb && rb->GetRigidBody() && (mYaw != 0 | mPitch != 0 | mRoll != 0))
		{
			btRigidBody* body = rb->GetRigidBody();
			body->activate(true);
			body->setAngularFactor(5.0f);
			//body->setLinearVelocity(btVector3(0.0f, 0.0f, 0.0f));
				
			btVector3 vel = body->getLinearVelocity();
			btVector3 target(mPitch * 1.0 * Timer::deltaTime, mRoll * 1.0 * Timer::deltaTime, mYaw * 1.0 * Timer::deltaTime);
			target.normalize();
			// float factor = 0.3f;
			// btVector3 smoothVel = vel.lerp(target, factor);
			body->applyTorque(target);
			ZP_CORE_INFO(Physics::FromBtVec3(target).ToString());
		}
		
		mYaw = 0.0f;
		mPitch = 0.0f;
		mRoll = 0.0f;
	}

	void TheDescentPC::SetMovementSpeed(float pSpeed)
	{
		mSpeed = pSpeed;
	}
	
	void TheDescentPC::Rotate(Vector2D delta)
	{
		mYaw = delta.x * mMouseSensitivity;
		mPitch = delta.y * -mMouseSensitivity;
	}

	void TheDescentPC::Move(Vector2D delta)
	{
		CameraComponent* cam = mOwner->GetComponentOfType<CameraComponent>();
		Vector3D forward = cam->GetWorldTransform().GetYAxis();
		Vector3D right   = cam->GetWorldTransform().GetXAxis();

		Vector3D moveDir = forward * delta.y + right * -delta.x;

		if (moveDir.LengthSq() > 0.0f)
		{
			auto rb = mOwner->GetRigidBody();
			if (rb && rb->GetRigidBody())
			{
				btRigidBody* body = rb->GetRigidBody();
				body->activate(true);
				//body->setLinearVelocity(btVector3(0.0f, 0.0f, 0.0f));
				
				btVector3 vel = body->getLinearVelocity();
				btVector3 target(moveDir.x * mSpeed * Timer::deltaTime, moveDir.y * mSpeed* Timer::deltaTime, 0);
				target.safeNormalize();
				// float factor = 0.3f;
				// btVector3 smoothVel = vel.lerp(target, factor);
				body->applyCentralImpulse(target);
			
				moveDir.Normalize();
			}
		}
	}

	void TheDescentPC::UpDown(float direction)
	{
		auto up = Vector3D::unitZ;
		mOwner->GetTransformComponent().Translate(up * direction * mSpeed * Timer::deltaTime);
	}

	void TheDescentPC::Tilt(float direction)
	{
		mRoll = direction * mTiltSpeed * Timer::deltaTime;
	}
}
