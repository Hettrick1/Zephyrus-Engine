#include "InputActions.h"
#include "IActionListener.h"

InputActions::InputActions(std::string name)
	: mName(name)
{
}

std::string InputActions::GetName()
{
	return mName;
}

void InputActions::SetName(std::string newName)
{
	mName = newName;
}

void InputActions::AddListener(IActionListener* listener)
{
	mListeners.push_back(listener);
}

void InputActions::RemoveListener(IActionListener* listenerToRemove)
{
	mListeners.erase(std::remove(mListeners.begin(), mListeners.end(), listenerToRemove), mListeners.end());
}

void InputActions::ClearListeners()
{
	mListeners.clear();
}

void InputActions::NotifyListenersStarted()
{
	for (IActionListener* listener : mListeners) {
		listener->OnActionStarted(this); 
	}
}

void InputActions::NotifyListenersTriggered()
{
	for (IActionListener* listener : mListeners) {
		listener->OnActionTriggered(this);
	}
}

void InputActions::NotifyListenersEnded()
{
	for (IActionListener* listener : mListeners) {
		listener->OnActionEnded(this);
	}
}
