#include "PlayerStartComponent.h"

PlayerStartComponent::PlayerStartComponent(Actor* pOwner, int updateOder)
	: Component(pOwner, "PlayerStartComponent", updateOder)
{
}

PlayerStartComponent::~PlayerStartComponent()
{
}

void PlayerStartComponent::Deserialize(const rapidjson::Value& pData)
{
	Component::Deserialize(pData);
	if (auto playerPrefabName = Serialization::Json::ReadString(pData, "playerPrefab"))
	{
		mPlayerPrefabName = *playerPrefabName;
	}
}

void PlayerStartComponent::Serialize(Serialization::Json::JsonWriter& pWriter)
{
	Component::BeginSerialize(pWriter);
	pWriter.WriteString("playerPrefab", mPlayerPrefabName);
	Component::EndSerialize(pWriter);
}

void PlayerStartComponent::OnEnd()
{
}
