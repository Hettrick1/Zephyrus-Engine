#pragma once
#include "ActionType.h"
#include "Vector2D.h"
#include <vector>

namespace Zephyrus::Inputs {
	class IActionListener;
	/// Base class for input actions, manages listeners and notifies them of action events.
	class InputActions
	{
	private:
		std::vector<IActionListener*> mListeners;
		std::string mName;

	public:
		InputActions(const std::string& pName);
		virtual ~InputActions() = default;

		virtual ActionType GetType() const = 0;
		virtual void Update() = 0;

		std::string GetName();
		void SetName(const std::string& pName);

		// Adds a listener to be notified of action events
		void AddListener(IActionListener* pListener);
		// Removes a listener from the notification list
		void RemoveListener(IActionListener* pListener);
		// Removes all listeners
		void ClearListeners();
	protected:
		// Notifies listeners that the action has started
		void NotifyListenersStarted();
		// Notifies listeners that the action has been triggered
		void NotifyListenersTriggered();
		// Notifies listeners that the action has ended
		void NotifyListenersEnded();
	};
}