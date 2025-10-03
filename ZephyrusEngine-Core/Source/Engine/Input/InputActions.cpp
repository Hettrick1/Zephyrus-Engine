#include "InputActions.h"
#include "IActionListener.h"
#include "Actor.h"

namespace Zephyrus::Inputs {
	InputActions::InputActions(const std::string& pName)
		: mName(pName)
	{
	}

	std::string InputActions::GetName()
	{
		return mName;
	}

	void InputActions::SetName(const std::string& pName)
	{
		mName = pName;
	}

	void InputActions::AddListener(IActionListener* pListener)
	{
		mListeners.push_back(pListener);
	}

	void InputActions::RemoveListener(IActionListener* pListener)
	{
		mListeners.erase(std::remove(mListeners.begin(), mListeners.end(), pListener), mListeners.end());
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
}
