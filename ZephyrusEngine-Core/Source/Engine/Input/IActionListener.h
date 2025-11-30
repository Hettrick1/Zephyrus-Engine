#pragma once
#include "InputAction.h"

namespace Zephyrus::Inputs {
	class IActionListener {
	public:
		virtual ~IActionListener() = default;
		virtual void OnActionStarted(InputAction* pAction) {}
		virtual void OnActionTriggered(InputAction* pAction) {}
		virtual void OnActionEnded(InputAction* pAction) {}
	};
}