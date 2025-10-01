#pragma once
#include "Event.h"
#include <string>

namespace Zephyrus::ActorComponent 
{
	class Actor;
}

class RenameActorEvent : public Event
{
private:
	std::string mNewName;
	std::string mPreviousName;
	Zephyrus::ActorComponent::Actor* mActorToRename = nullptr;
public:
	RenameActorEvent(Zephyrus::ActorComponent::Actor* pActorToRename, const std::string& pNewName);
	~RenameActorEvent();

	void Execute() override;
	void Undo() override;
};