#include "SpawnPrefabEvent.h"
#include "ISceneContext.h"
#include "PrefabFactory.h"

SpawnPrefabEvent::SpawnPrefabEvent(ISceneContext* pContext, const std::string& pPrefabToSpawn)
	: Event("Spawn Prefab Actor"), mContext(pContext), mPrefabToSpawn(pPrefabToSpawn)
{
}

SpawnPrefabEvent::~SpawnPrefabEvent()
{
}

void SpawnPrefabEvent::Execute()
{
	auto actor = mContext->GetPrefabFactory()->SpawnActorFromPrefab(mContext->GetActiveScene(), mPrefabToSpawn);
	actor->Start();
}

void SpawnPrefabEvent::Undo()
{
	//TODO
}
