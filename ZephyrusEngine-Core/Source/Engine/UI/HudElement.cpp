#include "pch.h"
#include "HudElement.h"
#include "SceneManager.h"
#include "HudManager.h"
#include "ISceneContext.h"

namespace Zephyrus::UI {
	HudElement::HudElement(ISceneContext* pContext)
		:mContext{ pContext }
	{
		mContext->GetRenderer()->GetHud()->AddElement(this);
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
}