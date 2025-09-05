#include "EventSystem.h"

std::vector<Event*> EventSystem::mAllEvents;
bool EventSystem::mCanUndo = false;

void EventSystem::DoEvent(Event* event)
{
	event->Execute();
	mAllEvents.push_back(event);
	if (!mAllEvents.empty())
	{
		mCanUndo = true;
	}
}

void EventSystem::UndoLastEvent()
{
	if (!mCanUndo)
	{
		return;
	}
	mAllEvents.back()->Undo();
	delete mAllEvents.back();
	mAllEvents.pop_back();
	if (mAllEvents.empty())
	{
		mCanUndo = false;
	}
}

void EventSystem::ClearAllEvents()
{
	for (auto event : mAllEvents)
	{
		delete event;
	}
}

size_t EventSystem::GetEventVectorSize()
{
	return mAllEvents.size();
}

bool EventSystem::GetCanUndo()
{
	return mCanUndo;
}
