#include "ShieldPickUp.h"
#include "Assets.h"
#include "SpriteComponent.h"
#include "BoxAABBComponent.h"
#include "DoomPlayer.h"
#include "Scene.h"
#include "CameraManager.h"
#include "CameraComponent.h"

ShieldPickUp::ShieldPickUp(Vector3D pPos, Vector3D pSize, Quaternion pRotation)
	:Actor(pPos, pSize, pRotation)
{
	Start();
	mScene.AddActor(this);
}

ShieldPickUp::~ShieldPickUp()
{
}

void ShieldPickUp::Start()
{
	Actor::Start();
	Texture* tex = Assets::LoadTexture("../Imports/Sprites/Doom/armorPu.png", "armorPickUp");
	SpriteComponent* sc = new SpriteComponent(this, *tex);
	sc->RelativeRotateX(90);
	BoxAABBComponent* bc = new BoxAABBComponent(this, 10, 0.5);
	bc->SetIsQuerry(true);
	bc->AddListener(this);
}

void ShieldPickUp::Update()
{
	Actor::Update();
	Vector3D camPos = CameraManager::Instance().GetCurrentCamera()->GetWorldTransform().GetTranslation();
	Vector3D direction = camPos - mTransformComponent.GetPosition();

	float angleZ = Maths::ATan2(direction.y, direction.x);
	angleZ -= Maths::ToRad(90);
	Quaternion targetRotation = Quaternion(Vector3D::unitZ, angleZ);
	mTransformComponent.SetRotation(targetRotation);
}

void ShieldPickUp::Destroy()
{
	Actor::Destroy();
}

void ShieldPickUp::OnTriggerEnter(ColliderComponent* collider, HitResult* infos)
{
	SetActive(ActorState::Paused);
	if (infos->HitActor->GetTag() == "Player")
	{
		DoomPlayer* player = static_cast<DoomPlayer*>(infos->HitActor);
		player->PickUpShield(10);
	}
}

void ShieldPickUp::OnTriggerStay(ColliderComponent* collider, HitResult* infos)
{
}

void ShieldPickUp::OnTriggerExit(ColliderComponent* collider, HitResult* infos)
{
}
