#include "PickUpComponent.h"
#include "Actor/Actor.h"
#include "DoomPlayerComponent.h"
#include "Physics/Bullet/CubeColliderComponent.h"
#include "SceneManager.h"

namespace Zephyrus::ActorComponent
{
	PickUpComponent::PickUpComponent(Actor* pOwner, int updateOder)
		: Component(pOwner, "PickUpComponent", updateOder), mType(PickUpType::Amo)
	{
	}

	PickUpComponent::~PickUpComponent()
	{
	}

	void PickUpComponent::Deserialize(const rapidjson::Value& pData)
	{
		Component::Deserialize(pData);
		if (pData.HasMember("type") && pData["type"].IsString())
		{
			std::string typeStr = pData["type"].GetString();
			SetPickUpType(StringToPickUpType(typeStr));
		}
	}

	void PickUpComponent::Serialize(Serialization::Json::JsonWriter& pWriter)
	{
		Component::BeginSerialize(pWriter);
		pWriter.WriteString("type", PickUpTypeToString(mType));
		Component::EndSerialize(pWriter);
	}

	void PickUpComponent::OnStart()
	{
		Component::OnStart();
		mOwner->GetComponentOfType<CubeColliderComponent>()->AddListener(this);
	}

	void PickUpComponent::Update()
	{
		Component::Update();
		//Vector3D camPos = CameraManager::Instance().GetCurrentCamera()->GetWorldTransform().GetTranslation();
		Vector3D camPos = Zephyrus::Scenes::SceneManager::ActiveScene->GetCameraManager()->GetActiveCamera()->GetWorldTransform().GetTranslation();
		Vector3D direction = camPos - mOwner->GetTransformComponent().GetPosition();

		float angleZ = Maths::ATan2(direction.y, direction.x);
		angleZ -= Maths::ToRad(90);
		Quaternion targetRotation = Quaternion(Vector3D::unitZ, angleZ);
		mOwner->SetRotation(targetRotation);
	}

	void PickUpComponent::OnEnd()
	{
		Component::OnEnd();
	}

	inline void PickUpComponent::SetPickUpType(const PickUpType& pType)
	{
		mType = pType;
	}

	inline PickUpType PickUpComponent::StringToPickUpType(const std::string& str) {
		if (str == "health") return PickUpType::Health;
		if (str == "shield") return PickUpType::Shield;
		if (str == "amo") return PickUpType::Amo;
		return PickUpType::Health;
	}

	inline const char* PickUpComponent::PickUpTypeToString(PickUpType state) {
		switch (state) {
		case PickUpType::Health: return "health";
		case PickUpType::Shield: return "shield";
		case PickUpType::Amo:   return "amo";
		}
		return "active";
	}

	void PickUpComponent::OnTriggerEnter(BulletColliderComponent* collider, HitResult* infos)
	{
		if (infos->HitActor->HasTag("Player"))
		{
			DoomPlayerComponent* playerComp = infos->HitActor->GetComponentOfType<DoomPlayerComponent>();
			if (!playerComp)
			{
				ZP_WARN("HitActor has no DoomPlayerComponent!");
				return;
			}
			switch (mType)
			{
			case PickUpType::Health:
			{
				mOwner->SetActive(ActorState::Paused);
				playerComp->PickUpHealth(20);
				break;
			}
			case PickUpType::Shield:
			{
				mOwner->SetActive(ActorState::Paused);
				playerComp->PickUpShield(15);
				break;
			}
			case PickUpType::Amo:
			{
				mOwner->SetActive(ActorState::Paused);
				playerComp->PickUpAmo(10);
				break;
			}
			}
		}
	}

	void PickUpComponent::OnTriggerStay(BulletColliderComponent* collider, HitResult* infos)
	{
	}

	void PickUpComponent::OnTriggerExit(BulletColliderComponent* collider, HitResult* infos)
	{
	}
}
