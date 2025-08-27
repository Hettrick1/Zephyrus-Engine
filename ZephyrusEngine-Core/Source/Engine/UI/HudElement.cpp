#include "HudElement.h"
#include "SceneManager.h"
#include "HudManager.h"

HudElement::HudElement()
{
	SceneManager::ActiveScene->GetRenderer()->GetHud()->AddElement(this);
}

void HudElement::SetPosition(const Vector2D& pPosition)
{
	mPosition = pPosition;
}

void HudElement::SetDrawOrder(float pDrawOrder)
{
	if (pDrawOrder >= 0)
	{
		mDrawOrder = pDrawOrder;
	}
}
