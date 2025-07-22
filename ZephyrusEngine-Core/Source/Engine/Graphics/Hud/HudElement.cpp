#include "HudElement.h"
#include "SceneManager.h"
#include "HudManager.h"

HudElement::HudElement()
{
	SceneManager::ActiveScene->GetRenderer()->GetHud()->AddElement(this);
}

void HudElement::SetPosition(float pX, float pY)
{
	mPositionX = x; mPositionY = y;
}
