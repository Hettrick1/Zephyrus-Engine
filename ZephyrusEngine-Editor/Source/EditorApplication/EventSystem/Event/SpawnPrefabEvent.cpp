#include "SpawnPrefabEvent.h"
#include "PrefabFactory.h"

SpawnPrefabEvent::SpawnPrefabEvent(const std::string& pPrefabToSpawn)
	: Event("Spawn Prefab Actor"), mPrefabToSpawn(pPrefabToSpawn)
{
}

SpawnPrefabEvent::~SpawnPrefabEvent()
{
}

void SpawnPrefabEvent::Execute()
{
	auto actor = PrefabFactory::CreateActorFromPrefab(mPrefabToSpawn);
	actor->Start();
}

void SpawnPrefabEvent::Undo()
{
	//TODO
}
