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
	auto actor = Zephyrus::Scenes::SceneManager::mPrefabFactory->SpawnActorFromPrefab(Zephyrus::Scenes::SceneManager::ActiveScene, mPrefabToSpawn);
	actor->Start();
}

void SpawnPrefabEvent::Undo()
{
	//TODO
}
