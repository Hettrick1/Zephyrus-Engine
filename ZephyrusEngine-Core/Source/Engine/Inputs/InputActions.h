#pragma once
#include "ActionType.h"
#include "SDL.h"
#include "Vector2D.h"
#include <vector>

class IActionListener;

/// Base class for input actions, manages listeners and notifies them of action events.
class InputActions
{
public :
	InputActions(std::string name);
	virtual ~InputActions() = default;

	virtual ActionType GetType() const = 0;
	virtual void Update() = 0;

	std::string GetName();
	void SetName(std::string newName);

	// Adds a listener to be notified of action events
	void AddListener(IActionListener* listener);
	// Removes a listener from the notification list
	void RemoveListener(IActionListener* listener);
	// Removes all listeners
	void ClearListeners();
protected:
	// Notifies listeners that the action has started
	void NotifyListenersStarted();
	// Notifies listeners that the action has been triggered
	void NotifyListenersTriggered();
	// Notifies listeners that the action has ended
	void NotifyListenersEnded();
private:
	std::vector<IActionListener*> mListeners;
	std::string mName;
};
