#pragma once

#include "EmptyActor.h"
#include <string>

class PrefabFactory
{
public:
	static EmptyActor* CreateActorFromPrefab(const std::string& pPrefabName);
	static std::vector<std::string> GetPrefabFiles(const std::string& folderPath);
	static Component* CreateAndAttachComponent(const rapidjson::Value& componentJson, EmptyActor* actor);
private:
};