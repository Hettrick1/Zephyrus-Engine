#include "BowlingPin.h"
#include "Scene.h"
#include "MeshComponent.h"
#include "RigidbodyComponent.h"
#include "Timer.h"
#include "Physics/CollisionManager.h"
#include "BoxAABBComponent.h"
#include "BoxSATComponent.h"

BowlingPin::BowlingPin(Vector3D pPos, Vector3D pSize, Quaternion pRotation)
	: Actor(pPos, pSize, pRotation)
{
}

BowlingPin::~BowlingPin()
{
}

void BowlingPin::Start()
{
	Actor::Start();
	Texture* tex = Assets::LoadTexture("../Imports/Sprites/pin.jpg", "pinTex");
	Mesh* mesh = Assets::LoadMesh("../Imports/Meshes/pin.obj", "pin");
	mesh->AddTexture(tex);
	MeshComponent* meshComp = new MeshComponent(this, mesh);
	meshComp->SetTextureIndex(1);
	RigidbodyComponent* rb = new RigidbodyComponent(this);
	BoxSATComponent* bc = new BoxSATComponent(this, 10, Vector3D(GetTransformComponent().GetSize().x * 1.5, GetTransformComponent().GetSize().y * 1.5, 2.4), Vector3D(0,0,2.0));
	mInitialPos = mTransformComponent.GetPosition();
}

void BowlingPin::Update()
{
	Actor::Update();
}

void BowlingPin::Destroy()
{
	Actor::Destroy();
}

void BowlingPin::OnTriggerEnter(ColliderComponent* collider, HitResult* infos)
{
}

void BowlingPin::OnTriggerStay(ColliderComponent* collider, HitResult* infos)
{
}

void BowlingPin::OnTriggerExit(ColliderComponent* collider, HitResult* infos)
{
}