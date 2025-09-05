#pragma once
#include "Event.h"
#include <string>

class Actor;

class RenameActorEvent : public Event
{
private:
	std::string mNewName;
	std::string mPreviousName;
	Actor* mActorToRename = nullptr;
public:
	RenameActorEvent(Actor* pActorToRename, const std::string& pNewName);
	~RenameActorEvent();

	void Execute() override;
	void Undo() override;
};