#pragma once
#include "Event/Event.h"
#include <vector>
#include <string>

class EventSystem
{
private:
	static std::vector<Event*> mAllEvents;
	static std::vector<Event*> mUndoedEvents;
	static bool mCanUndo;
	static bool mCanRedo;
public:
	static void DoEvent(Event* event);
	static void UndoLastEvent();
	static void RedoLastUndo();
	static void ClearAllEvents();
	static size_t GetEventVectorSize();
	static bool GetCanUndo();
	static bool GetCanRedo();
	static std::string GetRedoEventName();
	static std::string GetLastEventName();
};