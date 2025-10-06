#include "PlayerStartComponent.h"
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
		if (auto playerPrefabName = pReader.ReadString("playerPrefab"))
		{
			mPlayerPrefabName = *playerPrefabName;
		}
	}

	void PlayerStartComponent::Serialize(Serialization::ISerializer& pWriter)
	{
		Component::BeginSerialize(pWriter);
		pWriter.WriteString("playerPrefab", mPlayerPrefabName);
		Component::EndSerialize(pWriter);
	}

	void PlayerStartComponent::OnEnd()
	{
	}

	std::vector<PropertyDescriptor> PlayerStartComponent::GetProperties()
	{
		return {
			{ "Player Prefab : ", &mPlayerPrefabName, PropertyType::Prefab }
		};
	}
}
