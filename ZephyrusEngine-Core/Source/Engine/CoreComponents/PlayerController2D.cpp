#include "PlayerController2D.h"
#include "BooleanActions.h"
#include "FlipbookComponent.h"
#include "InputManager.h"
#include "Actor.h"
#include <iostream>

PlayerController2D::PlayerController2D(Actor* pOwner, int pUpdateOrder)
	: ActorMovementComponent(pOwner, pUpdateOrder), mCanGoRight(true), mCanGoLeft(true), mCanGoUp(true), mCanGoDown(true)
{
	InputManager& inputManager = InputManager::Instance();
	inputManager.CreateNewBooleanKeyBinding(this, "jump", SDLK_SPACE);
	inputManager.CreateNewBooleanKeyBinding(this, "up", SDLK_w);
	inputManager.CreateNewBooleanKeyBinding(this, "left", SDLK_a);
	inputManager.CreateNewBooleanKeyBinding(this, "down", SDLK_s);
	inputManager.CreateNewBooleanKeyBinding(this, "right", SDLK_d);
}

void PlayerController2D::OnActionStarted(InputActions* action)
{
	if (action->GetType() == ActionType::Boolean) {
		auto* Triggeredaction = dynamic_cast<BooleanActions*>(action);
		if (Triggeredaction && Triggeredaction->GetName() == "jump") {
			std::cout << "JUMP!\n";
		}
	}
}

void PlayerController2D::OnActionTriggered(InputActions* action)
{
	if (action->GetType() == ActionType::Boolean) 
	{
		auto* Triggeredaction = dynamic_cast<BooleanActions*>(action);
		if (Triggeredaction && Triggeredaction->GetName() == "up") 
		{
			if (mCanGoUp)
			{
				SetSpeedY(50);
			}
			else {
				SetSpeedY(0);
			}
		}
		else if (Triggeredaction && Triggeredaction->GetName() == "left") 
		{
			if (mCanGoLeft)
			{
				SetSpeedX(-50);
			}
			else {
				SetSpeedX(0);
			}
		}
		else if (Triggeredaction && Triggeredaction->GetName() == "down") 
		{
			if (mCanGoDown)
			{
				SetSpeedY(-50);
			}
			else {
				SetSpeedY(0);
			}
		}
		else if (Triggeredaction && Triggeredaction->GetName() == "right") 
		{
			if (mCanGoRight)
			{
				SetSpeedX(50);
			}
			else {
				SetSpeedX(0);
			}
		}
	}
}

void PlayerController2D::OnActionEnded(InputActions* action)
{
	if (action->GetType() == ActionType::Boolean) {
		auto* Triggeredaction = dynamic_cast<BooleanActions*>(action);
		if (Triggeredaction && Triggeredaction->GetName() == "up")
		{
			SetSpeedY(0);
		}
		else if (Triggeredaction && Triggeredaction->GetName() == "left")
		{
			SetSpeedX(0);
		}
		else if (Triggeredaction && Triggeredaction->GetName() == "down")
		{
			SetSpeedY(0);
		}
		else if (Triggeredaction && Triggeredaction->GetName() == "right")
		{
			SetSpeedX(0);
		}
	}
}

void PlayerController2D::Update()
{
	ActorMovementComponent::Update();
	FlipbookComponent* flipbook = mOwner->GetComponentOfType<FlipbookComponent>();
	if (flipbook)
	{
		if (mSpeed.x < 0) {
			flipbook->SetFlipMethode(IRenderer::Flip::Horizontal);
		}
		else if (mSpeed.x > 0) {
			flipbook->SetFlipMethode(IRenderer::Flip::None);
		}
		if (Maths::NearZero(mSpeed.Length())) {
			flipbook->SetAnimationFps(0);
		}
		else {
			flipbook->SetAnimationFps(5);
		}
	}
}

void PlayerController2D::EnableDirection(Vector2D pDirection)
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

void PlayerController2D::DisableDirection(Vector2D pDirection)
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
