#pragma once
#include "InputActions.h"

class IActionListener {
public:
	virtual ~IActionListener() = default;
	virtual void OnActionStarted(InputActions* pAction) = 0;
	virtual void OnActionTriggered(InputActions* pAction) = 0;
	virtual void OnActionEnded(InputActions* pAction) = 0;
};
