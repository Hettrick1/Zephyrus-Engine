#include "EventSystem.h"

std::vector<Event*> EventSystem::mAllEvents;
std::vector<Event*> EventSystem::mUndoedEvents;
bool EventSystem::mCanUndo = false;
bool EventSystem::mCanRedo = false;

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
	mUndoedEvents.push_back(mAllEvents.back());
	mAllEvents.pop_back();
	if (mAllEvents.empty())
	{
		mCanUndo = false;
	}
	mCanRedo = true;
}

void EventSystem::RedoLastUndo()
{
	if (!mCanRedo)
	{
		return;
	}
	DoEvent(mUndoedEvents.back());
	mUndoedEvents.pop_back();
	if (mUndoedEvents.empty())
	{
		mCanRedo = false;
	}
	mCanUndo = true;
}

void EventSystem::ClearAllEvents()
{
	for (auto event : mAllEvents)
	{
		delete event;
	}
	for (auto event : mUndoedEvents)
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

bool EventSystem::GetCanRedo()
{
	return mCanRedo;
}

std::string EventSystem::GetRedoEventName()
{
	if (mUndoedEvents.empty())
	{
		return "";
	}
	return mUndoedEvents.back()->GetEventName();
}

std::string EventSystem::GetLastEventName()
{
	if (mAllEvents.empty())
	{
		return "";
	}
	return mAllEvents.back()->GetEventName();
}
