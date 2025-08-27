#pragma once

#include "EmptyActor.h"
#include <string>

class PrefabFactory
{
public:
	static EmptyActor* CreateActorFromPrefab(const std::string& pPrefabName);
private:
	static Component* CreateAndAttachComponent(const rapidjson::Value& componentJson, EmptyActor* actor);
};