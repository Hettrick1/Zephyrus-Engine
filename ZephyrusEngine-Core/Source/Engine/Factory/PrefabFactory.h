#pragma once

#include "EmptyActor.h"
#include <string>

class PrefabFactory
{
public:
	static EmptyActor* CreateActorFromPrefab(const std::string& pPrefabName);
};