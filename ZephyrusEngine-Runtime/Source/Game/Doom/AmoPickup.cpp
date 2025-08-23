#include "AmoPickup.h"
#include "Assets.h"
#include "SpriteComponent.h"
#include "BoxAABBComponent.h"
#include "DoomPlayer.h"
#include "Scene.h"
#include "CameraManager.h"
#include "CameraComponent.h"

AmoPickup::AmoPickup(Vector3D pPos, Vector3D pSize, Quaternion pRotation)
	:Actor(pPos, pSize, pRotation)
{
	Start();
	mScene.AddActor(this);
}

AmoPickup::~AmoPickup()
{
}

void AmoPickup::Start()
{
	Actor::Start();
	Texture* tex = Assets::LoadTexture("Sprites/Doom/amoPu.png", "amoPickUp");
	/*SpriteComponent* sc = new SpriteComponent(this, *tex);
	sc->RelativeRotateX(90);*/
	/*BoxAABBComponent* bc = new BoxAABBComponent(this, 10, 0.5);
	bc->SetIsQuerry(true);
	bc->AddListener(this);*/
}

void AmoPickup::Update()
{
	Actor::Update();
	Vector3D camPos = CameraManager::Instance().GetCurrentCamera()->GetWorldTransform().GetTranslation();
	Vector3D direction = camPos - mTransformComponent.GetPosition();

	float angleZ = Maths::ATan2(direction.y, direction.x);
	angleZ -= Maths::ToRad(90);
	Quaternion targetRotation = Quaternion(Vector3D::unitZ, angleZ);
	mTransformComponent.SetRotation(targetRotation);
}

void AmoPickup::Destroy()
{
	Actor::Destroy();
}

void AmoPickup::OnTriggerEnter(ColliderComponent* collider, HitResult* infos)
{
	SetActive(ActorState::Paused);
	if (infos->HitActor->GetTag() == "Player")
	{
		DoomPlayer* player = static_cast<DoomPlayer*>(infos->HitActor);
		player->PickUpAmo(10);
	}
}

void AmoPickup::OnTriggerStay(ColliderComponent* collider, HitResult* infos)
{
}

void AmoPickup::OnTriggerExit(ColliderComponent* collider, HitResult* infos)
{
}
