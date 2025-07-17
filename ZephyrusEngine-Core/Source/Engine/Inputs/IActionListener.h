#pragma once
#include "InputActions.h"

class IActionListener {
public:
	virtual ~IActionListener() = default;
	virtual void OnActionStarted(InputActions* action) = 0;
	virtual void OnActionTriggered(InputActions* action) = 0;
	virtual void OnActionEnded(InputActions* action) = 0;
};
