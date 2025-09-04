#include "SpawnPrefabEvent.h"
#include "PrefabFactory.h"

SpawnPrefabEvent::SpawnPrefabEvent(const std::string& pPrefabToSpawn)
	: Event(), mPrefabToSpawn(pPrefabToSpawn)
{
}

SpawnPrefabEvent::~SpawnPrefabEvent()
{
}

void SpawnPrefabEvent::Execute()
{
	PrefabFactory::CreateActorFromPrefab(mPrefabToSpawn);
}

void SpawnPrefabEvent::Undo()
{
	//TODO
}
