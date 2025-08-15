#pragma once

#include "Engine/CoreActors/EmptyActor.h"
#include <string>

class PrefabFactory
{
public:
	static EmptyActor* CreateActorFromPrefab(const std::string& pPrefabName);
};