#include "SpawnPrefabEvent.h"
#include "PrefabFactory.h"
#include "SceneManager.h"

SpawnPrefabEvent::SpawnPrefabEvent(const std::string& pPrefabToSpawn)
	: Event("Spawn Prefab Actor"), mPrefabToSpawn(pPrefabToSpawn)
{
}

SpawnPrefabEvent::~SpawnPrefabEvent()
{
}

void SpawnPrefabEvent::Execute()
{
	auto actor = PrefabFactory::SpawnActorFromPrefab(mPrefabToSpawn);
	actor->Start();
}

void SpawnPrefabEvent::Undo()
{
	//TODO
}
