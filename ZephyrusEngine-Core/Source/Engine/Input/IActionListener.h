#pragma once
#include "InputActions.h"

namespace Zephyrus::Inputs {
	class IActionListener {
	public:
		virtual ~IActionListener() = default;
		virtual void OnActionStarted(InputActions* pAction) {}
		virtual void OnActionTriggered(InputActions* pAction) {}
		virtual void OnActionEnded(InputActions* pAction) {}
	};
}