#include "pch.h"
#include "TheDescentPCNoPhysics.h"

#include "InputActionBool.h"
#include "InputActionAxis2D.h"
#include "InputManager.h"
#include "Log.h"
#include "Timer.h"
#include "Actor.h"

namespace Zephyrus::ActorComponent {
	TheDescentPCNoPhysics::TheDescentPCNoPhysics(Actor* pOwner, int pUpdateOrder)
		: Component(pOwner, "TheDescentPCNoPhysics", pUpdateOrder)
	{
	}

	TheDescentPCNoPhysics::~TheDescentPCNoPhysics()
	{
		mInputManager = nullptr;
	}

	void TheDescentPCNoPhysics::Deserialize(Serialization::IDeserializer& pReader)
	{
		Component::Deserialize(pReader);
	}

	void TheDescentPCNoPhysics::Serialize(Serialization::ISerializer& pWriter)
	{
		Component::Serialize(pWriter);
	}

	void TheDescentPCNoPhysics::OnStart()
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
			move.BindKeyValue(GLFW_KEY_W, Vector2D(1.0f, 0.0f));
			move.BindKeyValue(GLFW_KEY_S, Vector2D(-1.0f, 0.0f));
			move.BindKeyValue(GLFW_KEY_A, Vector2D(0.0f, -1.0f));
			move.BindKeyValue(GLFW_KEY_D, Vector2D(0.0f, 1.0f));
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

	void TheDescentPCNoPhysics::Update()
	{
		Component::Update();

		Quaternion finalRot = Quaternion(Vector3D(mPitch, mRoll, mYaw));
		finalRot.Normalize();

		mOwner->GetTransformComponent().SetRotation(finalRot);
	}

	void TheDescentPCNoPhysics::SetMovementSpeed(float pSpeed)
	{
		mSpeed = pSpeed;
	}
	
	void TheDescentPCNoPhysics::Rotate(Vector2D delta)
	{
		mYaw += delta.x * mMouseSensitivity;
		mPitch += delta.y * -mMouseSensitivity;

		float yawRad = zpMaths::ToRad(mYaw);
		float pitchRad = zpMaths::ToRad(mPitch);
	}

	void TheDescentPCNoPhysics::Move(Vector2D delta)
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

	void TheDescentPCNoPhysics::UpDown(float direction)
	{
		auto up = Vector3D::unitZ;
		mOwner->GetTransformComponent().Translate(up * direction * mSpeed * Timer::deltaTime);
	}

	void TheDescentPCNoPhysics::Tilt(float direction)
	{
		mRoll += direction * mTiltSpeed * Timer::deltaTime;
	}
}
