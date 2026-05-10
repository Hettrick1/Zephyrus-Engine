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
#include "../../Engine/Rendering/OpenGl/DebugRenderer.h"
#include "../../Engine/Rendering/Assets/AssetsManager.h"
#include "../../Engine/UI/HudElement/HudImage.h"
#include "../../Engine/UI/HudElement/HudText.h"
#include "../../Scenes/Scene.h"
#include "../../Engine/Component/AIComponent/AiControllerComponent.h"

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

			auto& RightClick = mInputManager->CreateBool("RightClick");
			RightClick.BindMouseButton(GLFW_MOUSE_BUTTON_2);
			RightClick.OnStarted = [this]()
				{
					SelectPath();
				};

			auto& LeftClick = mInputManager->CreateBool("LeftClick");
			LeftClick.BindMouseButton(GLFW_MOUSE_BUTTON_1);
			LeftClick.OnStarted = [this]()
				{
					SelectNodeAndSendToAI();
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

			std::string MainMenuPngID = "e89d92d8-11f6-47a6-a6d8-48c8486cd80d";
			Assets::ITexture2D* damageIndicator = Zephyrus::Assets::AssetsManager::GetInstance().LoadTexture(MainMenuPngID);
			mCrossHair = new Zephyrus::UI::HudImage(mOwner->GetSceneContext(), damageIndicator, Vector2D(0, 0), 0.03);
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

	void ProjectResearchControllerComponent::SelectPath()
	{
		auto direction = mOwner->GetTransformComponent().Forward();
		auto start = GetWorldPosition();
		auto end = start + direction * 1000;

		HitResult hit;

		if (mOwner->GetSceneContext()->GetPhysicsWorld()->LineTrace(start, end, hit))
		{
			if (mStartingNode && mEndNode)
			{
				Matrix4DRow box1 = Matrix4DRow::CreateTranslation(mStartingNode->nodePosition);
				box1 *= Matrix4DRow::CreateScale(1.0);
				Vector3D color1 = Vector3D(1.0, 0.0, 1.0);
				mOwner->GetSceneContext()->GetRenderer()->GetDebugRenderer()->RemovePersistantDebugBox({ box1, color1, 3 });

				Matrix4DRow box2 = Matrix4DRow::CreateTranslation(mEndNode->nodePosition);
				box2 *= Matrix4DRow::CreateScale(1.0);
				Vector3D color2 = Vector3D(0.0, 0.5, 1.0);
				mOwner->GetSceneContext()->GetRenderer()->GetDebugRenderer()->RemovePersistantDebugBox({ box2, color2, 3 });

				mOwner->GetSceneContext()->GetRenderer()->GetDebugRenderer()->FlushDebugArrows(2);

				mStartingNode = nullptr;
				mEndNode = nullptr;
			}

			auto node = mOwner->GetSceneContext()->GetNavGridManager()->GetNearestNodeFromWorldPosition(hit.HitPoint);

			const bool SettingStartingNode = !mStartingNode;

			if (SettingStartingNode)
			{
				mStartingNode = node;
			}

			if (!SettingStartingNode)
			{
				mEndNode = node;

				auto path = mOwner->GetSceneContext()->GetNavGridManager()->GetShortestPath(mStartingNode, mEndNode);
				for (auto node : path)
				{
					if (node->parent) // starting from the second node
					{
						auto line = Debug::Debug2DArrow(node->parent->nodePosition.AddZ(0.2f), node->nodePosition.AddZ(0.2f));
						mOwner->GetSceneContext()->GetRenderer()->GetDebugRenderer()->AddDebugArrow(line, 2);
					}
				}
			}

			Matrix4DRow box = Matrix4DRow::CreateTranslation(node->nodePosition);
			box *= Matrix4DRow::CreateScale(1.0);

			Vector3D color = SettingStartingNode ? Vector3D(1.0, 0.0, 1.0) : Vector3D(0.0, 0.5, 1.0);

			mOwner->GetSceneContext()->GetRenderer()->GetDebugRenderer()->AddPersistantDebugBox({ box, color, 3 });
		}
	}
	void ProjectResearchControllerComponent::SelectNodeAndSendToAI()
	{
		auto direction = mOwner->GetTransformComponent().Forward();
		auto start = GetWorldPosition();
		auto end = start + direction * 1000;

		HitResult hit;

		mOwner->GetSceneContext()->GetRenderer()->GetDebugRenderer()->FlushDebugBoxes(5);

		if (mOwner->GetSceneContext()->GetPhysicsWorld()->LineTrace(start, end, hit))
		{
			auto actors = mOwner->GetScene().GetAllActors();
			for (auto& actor : actors)
			{
				if (actor->HasTag("AI"))
				{
					auto aiController = actor->GetComponentOfType<AiControllerComponent>();
					aiController->SetTarget(hit.HitPoint);
				}
			}
			auto node = mOwner->GetSceneContext()->GetNavGridManager()->GetNearestNodeFromWorldPosition(hit.HitPoint);
			Debug::DebugBox box = Debug::DebugBox(node->nodePosition, Vector3D(0.25), hit, Vector3D(0.0, 0.5, 1.0));
			mOwner->GetSceneContext()->GetRenderer()->GetDebugRenderer()->AddDebugBox(box, 5);
		}
	}
}
