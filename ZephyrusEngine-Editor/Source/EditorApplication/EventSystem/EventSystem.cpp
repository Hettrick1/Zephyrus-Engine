#include "pch.h"
#include "EventSystem.h"
#include "SceneManager.h"

std::vector<Event*> EventSystem::mAllEvents;
std::vector<Event*> EventSystem::mUndoedEvents;
bool EventSystem::mCanUndo = false;
bool EventSystem::mCanRedo = false;

void EventSystem::Update()
{
	ZP_EDITOR_WARN("number of events : " + std::to_string(mAllEvents.size()));
}

void EventSystem::DoEvent(Event* event)
{
	//Zephyrus::Scenes::SceneManager::ActiveScene->SetIsSaved(false);
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
	//Zephyrus::Scenes::SceneManager::ActiveScene->SetIsSaved(false);
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
	//Zephyrus::Scenes::SceneManager::ActiveScene->SetIsSaved(false);
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
	mUndoedEvents.clear();
	mAllEvents.clear(); 
	mCanUndo = false;
	mCanRedo = false;
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
