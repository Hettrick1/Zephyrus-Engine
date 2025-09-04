#pragma once

class Event
{
private:

public:
	Event();
	virtual ~Event();

	virtual void Execute() = 0;
	virtual void Undo() = 0;
};