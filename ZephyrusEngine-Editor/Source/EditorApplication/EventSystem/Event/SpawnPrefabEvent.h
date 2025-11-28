#pragma once
#include "Event.h"
#include <string>

class ISceneContext;

class SpawnPrefabEvent : public Event
{
private:
	ISceneContext* mContext{ nullptr };
	std::string mPrefabToSpawn;
public:
	SpawnPrefabEvent(ISceneContext* pContext, const std::string& pPrefabToSpawn);
	~SpawnPrefabEvent() override;

	void Execute() override;
	void Undo() override;
};