#include "BowlingBall.h"
#include "Scene.h"
#include "MeshComponent.h"
#include "RigidbodyComponent.h"
#include "Timer.h"
#include "Physics/CollisionManager.h"
#include "BoxSATComponent.h"

BowlingBall::BowlingBall(Vector3D pPos, Vector3D pSize, Quaternion pRotation)
	: Actor(pPos, pSize, pRotation)
{
}

BowlingBall::~BowlingBall()
{
}

void BowlingBall::Start()
{
	Actor::Start();
	Texture* tex = Assets::LoadTexture("../Imports/Sprites/ball.jpg", "sphere");
	Mesh* mesh = Assets::LoadMesh("../Imports/Meshes/sphere.obj", "sphere");
	mesh->AddTexture(tex);
	MeshComponent* meshComp = new MeshComponent(this, mesh);
	meshComp->SetTextureIndex(1);
	RigidbodyComponent* rb = new RigidbodyComponent(this);
	BoxSATComponent* bc = new BoxSATComponent(this, 10, GetTransformComponent().GetSize());
	SetPosition(Vector3D(mTransformComponent.GetPosition().x, mTransformComponent.GetPosition().y, mTransformComponent.GetPosition().z + 0.5));
}

void BowlingBall::Update()
{
	Actor::Update();
	if (GetRigidBody()->GetVelocity().LengthSq() > 0.1) {
		GetTransformComponent().RotateX(-GetRigidBody()->GetVelocity().LengthSq() * 2 * Timer::deltaTime);
	}
}

void BowlingBall::Destroy()
{
	Actor::Destroy();
}

void BowlingBall::OnTriggerEnter(ColliderComponent* collider, HitResult* infos)
{
}

void BowlingBall::OnTriggerStay(ColliderComponent* collider, HitResult* infos)
{
}

void BowlingBall::OnTriggerExit(ColliderComponent* collider, HitResult* infos)
{
}
