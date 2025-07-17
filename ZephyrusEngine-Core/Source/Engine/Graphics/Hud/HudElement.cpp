#include "HudElement.h"
#include "SceneManager.h"
#include "HudManager.h"

HudElement::HudElement()
{
	SceneManager::ActiveScene->GetRenderer()->GetHud()->AddElement(this);
}

void HudElement::SetPosition(float x, float y)
{
	mPositionX = x; mPositionY = y;
}
