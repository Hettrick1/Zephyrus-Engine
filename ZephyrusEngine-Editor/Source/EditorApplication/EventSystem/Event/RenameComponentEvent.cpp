#include "pch.h"
#include "RenameComponentEvent.h"
#include "Actor.h"

RenameComponentEvent::RenameComponentEvent(Zephyrus::ActorComponent::Component* pComponentToRename, const std::string& pNewName)
	: Event("Rename Actor"), mNewName(pNewName), mPreviousName(pComponentToRename->GetName()), mComponentToRename(pComponentToRename)
{
}

RenameComponentEvent::~RenameComponentEvent()
{
	mComponentToRename = nullptr;
}

void RenameComponentEvent::Execute()
{
	mComponentToRename->SetName(mNewName);
}

void RenameComponentEvent::Undo()
{
	mComponentToRename->SetName(mPreviousName);
}
