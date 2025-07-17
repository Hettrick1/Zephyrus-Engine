#include "Sphere.h"
#include "Scene.h"
#include "MeshComponent.h"
#include "RigidbodyComponent.h"
#include "Timer.h"
#include "Physics/CollisionManager.h"
#include "BoxAABBComponent.h"

Sphere::Sphere(Vector3D pPos, Vector3D pSize, Quaternion pRotation)
	: Actor(pPos, pSize, pRotation)
{
}

Sphere::~Sphere()
{
}

void Sphere::Start()
{
	Actor::Start();
	Texture* tex = Assets::LoadTexture(*GetScene().GetRenderer(), "../Imports/Sprites/LakeSkyTexture.png", "sphere");
	Mesh* mesh = Assets::LoadMesh("../Imports/Meshes/sphere.obj", "sphere");
	mesh->AddTexture(tex);
	MeshComponent* meshComp = new MeshComponent(this, mesh);
	meshComp->SetTextureIndex(1);
	AddComponent(meshComp);
	RigidbodyComponent* rb = new RigidbodyComponent(this);
	AddComponent(rb);
	BoxAABBComponent* bc = new BoxAABBComponent(this, 10, GetTransformComponent().GetSize());
	AddComponent(bc);
}

void Sphere::Update()
{
	Actor::Update();
}

void Sphere::Destroy()
{
	Actor::Destroy();
}

void Sphere::OnTriggerEnter(ColliderComponent* collider, HitResult* infos)
{
}

void Sphere::OnTriggerStay(ColliderComponent* collider, HitResult* infos)
{
}

void Sphere::OnTriggerExit(ColliderComponent* collider, HitResult* infos)
{
}
