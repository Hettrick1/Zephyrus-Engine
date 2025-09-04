#include "EventSystem.h"

std::vector<Event*> EventSystem::mAllEvents;

void EventSystem::DoEvent(Event* event)
{
	event->Execute();
	mAllEvents.push_back(event);
}

void EventSystem::UndoLastEvent()
{
	mAllEvents.back()->Undo();
	mAllEvents.pop_back();
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
