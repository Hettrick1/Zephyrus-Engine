#pragma once
#include "Event/Event.h"
#include <vector>
#include <string>

class EventSystem
{
private:
	static std::vector<Event*> mAllEvents;
	static bool mCanUndo;
public:
	static void DoEvent(Event* event);
	static void UndoLastEvent();
	static void ClearAllEvents();
	static size_t GetEventVectorSize();
	static bool GetCanUndo();
	static std::string GetLastEventName();
};