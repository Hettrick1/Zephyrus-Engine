#pragma once
#include "Event.h"
#include <string>

class SpawnPrefabEvent : public Event
{
private:
	std::string mPrefabToSpawn;
public:
	SpawnPrefabEvent(const std::string& pPrefabToSpawn);
	~SpawnPrefabEvent();

	void Execute() override;
	void Undo() override;
};