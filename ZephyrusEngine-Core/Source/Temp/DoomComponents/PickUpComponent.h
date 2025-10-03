#pragma once
#include "Component.h"
#include "Physics/ICollisionListener.h"

namespace Zephyrus::ActorComponent
{
	enum class PickUpType
	{
		Health,
		Shield,
		Amo
	};

	class PickUpComponent : public Component, public Zephyrus::Physics::ICollisionListener
	{
	private:
		PickUpType mType;

	public:
		PickUpComponent(Actor* pOwner, int updateOder = 0);
		~PickUpComponent();

		void Deserialize(const rapidjson::Value& pData) override;
		void Serialize(Serialization::Json::JsonWriter& pWriter) override;

		static Component* Create(Actor* pOwner) { return new PickUpComponent(pOwner); }

		void OnStart() override;
		void Update() override;
		void OnEnd() override;

		inline void SetPickUpType(const PickUpType& pType);

		inline PickUpType StringToPickUpType(const std::string& str);

		inline const char* PickUpTypeToString(PickUpType state);

	public:
		void OnTriggerEnter(BulletColliderComponent* collider, HitResult* infos) override;
		void OnTriggerStay(BulletColliderComponent* collider, HitResult* infos) override;
		void OnTriggerExit(BulletColliderComponent* collider, HitResult* infos) override;
	};
}