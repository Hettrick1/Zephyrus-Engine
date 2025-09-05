#pragma once
#include <string>

class Event
{
private:
	std::string mEventName;
public:
	Event(const std::string& pEventName);
	virtual ~Event();

	virtual void Execute() = 0;
	virtual void Undo() = 0;

	inline std::string GetEventName() const { return mEventName; }
};