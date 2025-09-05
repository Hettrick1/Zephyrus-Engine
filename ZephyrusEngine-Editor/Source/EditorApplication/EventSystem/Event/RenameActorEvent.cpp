#include "RenameActorEvent.h"
#include "Actor.h"

RenameActorEvent::RenameActorEvent(Actor* pActorToRename, const std::string& pNewName)
	: Event("Rename Actor"), mNewName(pNewName), mPreviousName(pActorToRename->GetName()), mActorToRename(pActorToRename)
{
}

RenameActorEvent::~RenameActorEvent()
{
}

void RenameActorEvent::Execute()
{
	mActorToRename->SetName(mNewName);
}

void RenameActorEvent::Undo()
{
	mActorToRename->SetName(mPreviousName);
}
