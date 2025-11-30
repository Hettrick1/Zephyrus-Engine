#include "pch.h"
#include "RenameActorEvent.h"
#include "Actor.h"

RenameActorEvent::RenameActorEvent(Zephyrus::ActorComponent::Actor* pActorToRename, const std::string& pNewName)
	: Event("Rename Actor"), mNewName(pNewName), mPreviousName(pActorToRename->GetName()), mActorToRename(pActorToRename)
{
}

RenameActorEvent::~RenameActorEvent()
{
	mActorToRename = nullptr;
}

void RenameActorEvent::Execute()
{
	mActorToRename->SetName(mNewName);
}

void RenameActorEvent::Undo()
{
	mActorToRename->SetName(mPreviousName);
}
