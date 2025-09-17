#pragma once

#include "EmptyActor.h"
#include <string>

class PrefabFactory
{
public:
	static EmptyActor* SpawnActorFromPrefab(const std::string& pPrefabName, const Vector3D& pInitialPos = 0, const Vector3D& pInitialRot = 0, const Vector3D& pInitialSize = 1);
	static EmptyActor* InitPrefab(const std::string& pPrefabName);
	static std::vector<std::string> GetPrefabFiles(const std::string& folderPath);
	static Component* CreateAndAttachComponent(const rapidjson::Value& componentJson, EmptyActor* actor, bool doDeserialize = true);
private:
};