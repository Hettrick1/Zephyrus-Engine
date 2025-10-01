#pragma once

#include "EmptyActor.h"
#include <string>

class PrefabFactory
{
public:
	static Zephyrus::ActorComponent::EmptyActor* SpawnActorFromPrefab(const std::string& pPrefabName, const Vector3D& pInitialPos = 0, const Vector3D& pInitialRot = 0, const Vector3D& pInitialSize = 1);
	static Zephyrus::ActorComponent::EmptyActor* InitPrefab(const std::string& pPrefabName);
	static std::vector<std::string> GetPrefabFiles(const std::string& folderPath);
	static Zephyrus::ActorComponent::Component* CreateAndAttachComponent(const rapidjson::Value& componentJson, Zephyrus::ActorComponent::EmptyActor* actor, bool doDeserialize = true);
private:
};