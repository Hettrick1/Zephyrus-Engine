#pragma once
#include "Event/Event.h"
#include <vector>

class EventSystem
{
private:
	static std::vector<Event*> mAllEvents;
public:
	static void DoEvent(Event* event);
	static void UndoLastEvent();
	static void ClearAllEvents();
	static size_t GetEventVectorSize();
};