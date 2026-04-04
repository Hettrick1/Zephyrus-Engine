#include "pch.h"
#include "PlayerStartComponent.h"

#include "AssetsManager.h"

namespace Zephyrus::ActorComponent
{
	PlayerStartComponent::PlayerStartComponent(Actor* pOwner, int updateOder)
		: Component(pOwner, "PlayerStartComponent", updateOder)
	{
	}

	PlayerStartComponent::~PlayerStartComponent()
	{
	}

	void PlayerStartComponent::Deserialize(Serialization::IDeserializer& pReader)
	{
		Component::Deserialize(pReader);
		if (auto playerPrefabId = pReader.ReadString("playerPrefab"))
		{
			mPlayerPrefabId = *playerPrefabId;
		}
	}

	void PlayerStartComponent::Serialize(Serialization::ISerializer& pWriter)
	{
		Component::BeginSerialize(pWriter);
		pWriter.WriteString("playerPrefab", mPlayerPrefabId);
		Component::EndSerialize(pWriter);
	}

	void PlayerStartComponent::OnEnd()
	{
	}

	std::vector<PropertyDescriptor> PlayerStartComponent::GetProperties()
	{
		return {
			{ "Player Prefab : ", &mPlayerPrefabId, PropertyType::Prefab }
		};
	}
}
