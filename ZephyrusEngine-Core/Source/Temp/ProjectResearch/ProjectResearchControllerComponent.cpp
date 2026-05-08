#include "pch.h"
#include "ProjectResearchControllerComponent.h"

#include "InputActionBool.h"
#include "InputActionAxis2D.h"
#include "InputManager.h"
#include "Log.h"
#include "Timer.h"
#include "Actor.h"
#include "CameraComponent.h"

#include "../../Engine/Physics/Bullet/PhysicWorld.h"
#include "../../Engine/AI/NavGridManager.h"
#include "../../Engine/Rendering/OpenGl/DebugRenderer.h"
#include "../../Engine/Rendering/Assets/AssetsManager.h"
#include "../../Engine/UI/HudElement/HudImage.h"

namespace Zephyrus::ActorComponent {
	ProjectResearchControllerComponent::ProjectResearchControllerComponent(Actor* pOwner, int pUpdateOrder)
		: Component(pOwner, "ProjectResearchControllerComponent", pUpdateOrder)
	{
	}

	ProjectResearchControllerComponent::~ProjectResearchControllerComponent()
	{
		mInputManager = nullptr;
	}

	void ProjectResearchControllerComponent::Deserialize(Serialization::IDeserializer& pReader)
	{
		Component::Deserialize(pReader);
	}

	void ProjectResearchControllerComponent::Serialize(Serialization::ISerializer& pWriter)
	{
		Component::Serialize(pWriter);
	}

	void ProjectResearchControllerComponent::OnStart()
	{
		mInputManager = mOwner->GetSceneContext()->GetInputManager();
		if (mInputManager)
		{
			mInputManager->SetCursorRelative(true);
			auto& camera = mInputManager->CreateAxis2D("Camera");
			camera.IsMouseAxis = true;
			camera.OnTriggered = [this](Vector2D delta) { Rotate(delta); };

			auto& click = mInputManager->CreateBool("Click");
			click.BindMouseButton(GLFW_MOUSE_BUTTON_1);
			click.OnStarted = [this]()
				{
					SelectNode();
				};

			auto& move = mInputManager->CreateAxis2D("Move");
			move.BindKeyValue(GLFW_KEY_W, Vector2D(1.0f, 0.0f));
			move.BindKeyValue(GLFW_KEY_S, Vector2D(-1.0f, 0.0f));
			move.BindKeyValue(GLFW_KEY_A, Vector2D(0.0f, -1.0f));
			move.BindKeyValue(GLFW_KEY_D, Vector2D(0.0f, 1.0f));
			move.OnTriggered = [this](Vector2D delta) { Move(delta); };

			auto& upDown = mInputManager->CreateAxis1D("UpDown");
			upDown.OnTriggered = [this](float delta) { UpDown(delta); };
			upDown.BindKeyValue(GLFW_KEY_SPACE, 1.0f);
			upDown.BindKeyValue(GLFW_KEY_LEFT_SHIFT, -1.0f);

			std::string MainMenuPngID = "7387b94d-0c8a-4b75-83b7-da36a90f77f5";
			Assets::ITexture2D* damageIndicator = Zephyrus::Assets::AssetsManager::GetInstance().LoadTexture(MainMenuPngID);
			mCrossHair = new Zephyrus::UI::HudImage(mOwner->GetSceneContext(), damageIndicator, Vector2D(0, 0), 0.5);
			mCrossHair->SetTint(Vector4D(1.0, 1.0, 1.0, 1.0));
		}
	}

	void ProjectResearchControllerComponent::Update()
	{
		Component::Update();
	}

	void ProjectResearchControllerComponent::SetMovementSpeed(float pSpeed)
	{
		mSpeed = pSpeed;
	}

	void ProjectResearchControllerComponent::Rotate(Vector2D delta)
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

	void ProjectResearchControllerComponent::Move(Vector2D delta)
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

	void ProjectResearchControllerComponent::UpDown(float direction)
	{
		auto up = Vector3D::unitZ;
		mOwner->GetTransformComponent().Translate(up * direction * mSpeed * Timer::deltaTime);
	}
	void ProjectResearchControllerComponent::SelectNode()
	{
		ZP_CORE_INFO("SELECTING");

		auto direction = mOwner->GetTransformComponent().Forward();
		auto start = GetWorldPosition();
		auto end = start + direction * 1000;

		HitResult hit;

		if (mOwner->GetSceneContext()->GetPhysicsWorld()->LineTrace(start, end, hit))
		{
			auto node = mOwner->GetSceneContext()->GetNavGridManager()->GetNearestNodeFromWorldPosition(hit.HitPoint);

			Matrix4DRow box = Matrix4DRow::CreateTranslation(node->nodePosition);
			box *= Matrix4DRow::CreateScale(1.0);

			ZP_CORE_INFO("CREATING A BOX");

			mOwner->GetSceneContext()->GetRenderer()->GetDebugRenderer()->AddDebugBox(box);
		}
	}
}
