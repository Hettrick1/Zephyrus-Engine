#pragma once
#include "Event.h"
#include <string>

namespace Zephyrus::ActorComponent
{
	class Component;
}

class RenameComponentEvent : public Event
{
private:
	std::string mNewName;
	std::string mPreviousName;
	Zephyrus::ActorComponent::Component* mComponentToRename{ nullptr };
public:
	RenameComponentEvent(Zephyrus::ActorComponent::Component* pComponentToRename, const std::string& pNewName);
	~RenameComponentEvent() override;

	void Execute() override;
	void Undo() override;
};