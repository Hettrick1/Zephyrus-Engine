#include "pch.h"
#include "PlayerController2D.h"
#include "InputActionBool.h"
#include "FlipbookComponent.h"
#include "InputManager.h"
#include "Actor.h"
#include <iostream>

namespace Zephyrus::ActorComponent
{
	PlayerController2D::PlayerController2D(Actor* pOwner, int pUpdateOrder)
		: Component(pOwner, "PlayerController2D", pUpdateOrder), mCanGoRight(true), mCanGoLeft(true), mCanGoUp(true), mCanGoDown(true)
	{
		// InputManager& inputManager = InputManager::Instance();
		// inputManager.CreateNewBooleanKeyBinding(this, "jump", SDLK_SPACE);
		// inputManager.CreateNewBooleanKeyBinding(this, "up", SDLK_w);
		// inputManager.CreateNewBooleanKeyBinding(this, "left", SDLK_a);
		// inputManager.CreateNewBooleanKeyBinding(this, "down", SDLK_s);
		// inputManager.CreateNewBooleanKeyBinding(this, "right", SDLK_d);
	}

	void PlayerController2D::OnActionStarted(InputAction* pAction)
	{
		
	}

	void PlayerController2D::OnActionTriggered(InputAction* pAction)
	{
	}

	void PlayerController2D::OnActionEnded(InputAction* pAction)
	{
	}

	void PlayerController2D::Update()
	{
		Component::Update();
	}

	void PlayerController2D::EnableDirection(const Vector2D& pDirection)
	{
		if (pDirection.x < 0)
		{
			mCanGoLeft = true;
		}
		if (pDirection.x > 0)
		{
			mCanGoRight = true;
		}
		if (pDirection.y < 0)
		{
			mCanGoDown = true;
		}
		if (pDirection.y > 0)
		{
			mCanGoUp = true;
		}
	}

	void PlayerController2D::DisableDirection(const Vector2D& pDirection)
	{
		if (pDirection.x < 0)
		{
			mCanGoLeft = false;
		}
		if (pDirection.x > 0)
		{
			mCanGoRight = false;
		}
		if (pDirection.y < 0)
		{
			mCanGoDown = false;
		}
		if (pDirection.y > 0)
		{
			mCanGoUp = false;
		}
	}
}
