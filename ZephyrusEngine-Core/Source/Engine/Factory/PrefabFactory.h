#pragma once

#include "EmptyActor.h"
#include <string>

namespace Zephyrus::Debug
{
	class Log;
}

using Zephyrus::ActorComponent::Actor;
using Zephyrus::ActorComponent::Component;
using Zephyrus::Debug::Log;

class PrefabFactory
{
public:
	static Actor* SpawnActorFromPrefab(const std::string& pPrefabName, const Vector3D& pInitialPos = 0, const Vector3D& pInitialRot = 0, const Vector3D& pInitialSize = 1);
	static Actor* InitPrefab(const std::string& pPrefabName);
	static std::vector<std::string> GetPrefabFiles(const std::string& folderPath);
	static Component* CreateAndAttachComponent(const rapidjson::Value& componentJson, Actor* actor, bool doDeserialize = true);
};