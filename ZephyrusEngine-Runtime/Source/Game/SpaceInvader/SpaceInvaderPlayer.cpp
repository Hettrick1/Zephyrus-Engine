#include "SpaceInvaderPlayer.h"
#include "Scene.h"
#include "BooleanActions.h"
#include "InputManager.h"
#include "PlayerController2D.h"
#include "Physics/CollisionManager.h"
#include "BoxCollider2DComponent.h"


SpaceInvaderPlayer::SpaceInvaderPlayer() :
	Actor()
{
}

SpaceInvaderPlayer::~SpaceInvaderPlayer()
{
}

void SpaceInvaderPlayer::Start()
{
	Actor::Start();
	std::vector<Texture*> walkAnimTextures = {
		Assets::LoadTexture(*GetScene().GetRenderer(), "../Imports/Sprites/WalkAnim/Walk1.png", "walk1"),
		Assets::LoadTexture(*GetScene().GetRenderer(), "../Imports/Sprites/WalkAnim/Walk2.png", "walk2"),
		Assets::LoadTexture(*GetScene().GetRenderer(), "../Imports/Sprites/WalkAnim/Walk3.png", "walk3"),
		Assets::LoadTexture(*GetScene().GetRenderer(), "../Imports/Sprites/WalkAnim/Walk4.png", "walk4"),
		Assets::LoadTexture(*GetScene().GetRenderer(), "../Imports/Sprites/WalkAnim/Walk5.png", "walk5")
	};
	//CollisionManager::Instance().CreateCollider<BoxCollider2DComponent>(this, 10, (GetTransformComponent().GetSize() * 64) / 1);

	FlipbookComponent* walkAnim = new FlipbookComponent(this, walkAnimTextures, 500000);
	walkAnim->SetAnimationFps(5);
	AddComponent(walkAnim);

	PlayerController2D* pc = new PlayerController2D(this, 100); 
	AddComponent(pc);
}

void SpaceInvaderPlayer::Update()
{
	Actor::Update();
}

void SpaceInvaderPlayer::Destroy()
{
	Actor::Destroy();
}

void SpaceInvaderPlayer::OnTriggerEnter(ColliderComponent* collider, HitResult* infos)
{
 //   PlayerController2D* pc = GetComponentOfType<PlayerController2D>();
 //   Actor* otherActor = collider->GetHitResult().hitActor;

 //   float playerX = GetTransformComponent().GetPosition().x;
 //   float playerY = GetTransformComponent().GetPosition().y;
 //   float otherX = otherActor->GetTransformComponent().GetPosition().x;
 //   float otherY = otherActor->GetTransformComponent().GetPosition().y;

	//// calculate if the collider is on the left/right/up/down
	//float rightLeft = playerX - otherX;
	//float upDown = playerY - otherY;
	//float distX = Maths::Abs(rightLeft);
	//float distY = Maths::Abs(upDown);

	//// disable the input depending on where the collision
	//if (distX > distY) 
	//{ // collision up or down
	//	if (upDown < 0) // down
	//	{
	//		pc->DisableDirection({ 0, -1 });
	//	}
	//	else if (upDown > 0) // up
	//	{
	//		pc->DisableDirection({ 0, 1 });
	//	}
	//}
	//else if (distX < distY) 
	//{ // collision right or left
	//	if (rightLeft < 0) // left
	//	{
	//		pc->DisableDirection({ 1, 0 });
	//	}
	//	else if (rightLeft > 0) // right
	//	{
	//		pc->DisableDirection({ -1, 0 });
	//	}
	//}
}

void SpaceInvaderPlayer::OnTriggerStay(ColliderComponent* collider, HitResult* infos)
{
}

void SpaceInvaderPlayer::OnTriggerExit(ColliderComponent* collider, HitResult* infos)
{
	//PlayerController2D* pc = GetComponentOfType<PlayerController2D>(); 
	//Actor* otherActor = collider->GetHitResult().hitActor; 
	//if (!otherActor) {
	//	Log::Error(LogType::Error, "The other actor was nullptr");
	//	return; 
	//}

	//float playerX = GetTransformComponent().GetPosition().x; 
	//float playerY = GetTransformComponent().GetPosition().y; 
	//float otherX = otherActor->GetTransformComponent().GetPosition().x; 
	//float otherY = otherActor->GetTransformComponent().GetPosition().y; 

	//// calculate if the collider is on the left/right/up/down
	//float rightLeft = playerX - otherX; 
	//float upDown = playerY - otherY; 
	//float distX = Maths::Abs(rightLeft); 
	//float distY = Maths::Abs(upDown); 

	//// enable the inputs depending where de collision was
	//if (distX > distY)
	//{ // collision up or down
	//	if (upDown < 0)
	//	{
	//		pc->EnableDirection({ 0, -1 });
	//	}
	//	else if (upDown > 0)
	//	{
	//		pc->EnableDirection({ 0, 1 });
	//	}
	//}
	//if (distX < distY)
	//{ // collision right or left
	//	if (rightLeft < 0)
	//	{
	//		pc->EnableDirection({ 1, 0 });
	//	}
	//	else if (rightLeft > 0)
	//	{
	//		pc->EnableDirection({ -1, 0 });
	//	}
	//}
}


