#include "PickUpComponent.h"
#include "Actor/Actor.h"
#include "CameraManager.h"
#include "CameraComponent.h"

PickUpComponent::PickUpComponent(Actor* pOwner, int updateOder)
	: Component(pOwner, updateOder), mType(PickUpType::Amo)
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

void PickUpComponent::OnStart()
{
	Component::OnStart();
	mOwner->GetComponentOfType<ColliderComponent>()->AddListener(this);
}

void PickUpComponent::Update()
{
	Component::Update();
	Vector3D camPos = CameraManager::Instance().GetCurrentCamera()->GetWorldTransform().GetTranslation();
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

void PickUpComponent::OnTriggerEnter(ColliderComponent* collider, HitResult* infos)
{
	if (infos->HitActor->GetTag() == "Player")
	{
		switch (mType)
		{
		case PickUpType::Health:
			{
				break;
			}
		case PickUpType::Shield:
			{
				break;
			}
		case PickUpType::Amo:
			{
				mOwner->SetActive(ActorState::Paused);
				//DoomPlayer* player = static_cast<DoomPlayer*>(infos->HitActor);
				//player->PickUpAmo(10);
				break;
			}
		}
	}
}

void PickUpComponent::OnTriggerStay(ColliderComponent* collider, HitResult* infos)
{
}

void PickUpComponent::OnTriggerExit(ColliderComponent* collider, HitResult* infos)
{
}
