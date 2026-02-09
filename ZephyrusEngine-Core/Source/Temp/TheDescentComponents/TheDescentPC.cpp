#include "pch.h"
#include "TheDescentPC.h"

#include "InputActionBool.h"
#include "InputActionAxis2D.h"
#include "InputManager.h"
#include "Log.h"
#include "Timer.h"
#include "Actor.h"

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
			move.BindKeyValue(GLFW_KEY_W, Vector2D(1.0f, 0.0f));
			move.BindKeyValue(GLFW_KEY_S, Vector2D(-1.0f, 0.0f));
			move.BindKeyValue(GLFW_KEY_A, Vector2D(0.0f, -1.0f));
			move.BindKeyValue(GLFW_KEY_D, Vector2D(0.0f, 1.0f));
			move.OnTriggered = [this](Vector2D delta){ Move(delta); };
			
			auto& upDown = mInputManager->CreateAxis1D("UpDown");
			upDown.OnTriggered = [this](float delta){ UpDown(delta); };
			upDown.BindKeyValue(GLFW_KEY_SPACE, 1.0f);
			upDown.BindKeyValue(GLFW_KEY_LEFT_SHIFT, -1.0f);
		}
	}

	void TheDescentPC::Update()
	{

	}

	void TheDescentPC::SetMovementSpeed(float pSpeed)
	{
		mSpeed = pSpeed;
	}
	
	void TheDescentPC::Rotate(Vector2D delta)
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

	void TheDescentPC::Move(Vector2D delta)
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

	void TheDescentPC::UpDown(float direction)
	{
		auto up = Vector3D::unitZ;
		mOwner->GetTransformComponent().Translate(up * direction * mSpeed * Timer::deltaTime);
	}
}
