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
	auto actor = PrefabFactory::CreateActorFromPrefab(mPrefabToSpawn);
	SceneManager::ActiveScene->AddActor(actor);
	actor->Start();
}

void SpawnPrefabEvent::Undo()
{
	//TODO
}
