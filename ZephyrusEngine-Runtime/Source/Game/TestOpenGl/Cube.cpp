#include "Cube.h"
#include "Scene.h"
#include "MeshComponent.h"
#include "RigidbodyComponent.h"
#include "Timer.h"
#include "Physics/CollisionManager.h"
#include "BoxAABBComponent.h"

Cube::Cube(Vector3D pPos, Vector3D pSize, Quaternion pRotation)
	: Actor(pPos, pSize, pRotation)
{
}

Cube::~Cube()
{
}

void Cube::Start()
{
	Actor::Start();
	Texture* tex1 = Assets::LoadTexture("Sprites/Doom/268.png", "wallTex");
	Texture* tex2 = Assets::LoadTexture("Sprites/Doom/091.png", "groundTex");
	Texture* tex3 = Assets::LoadTexture("Sprites/Doom/268bis.png", "wallTex2");
	Mesh* mesh = Assets::LoadMesh("cube.obj", "cube");
	mesh->AddTexture(tex1);
	mesh->AddTexture(tex2);
	mesh->AddTexture(tex3);
	/*MeshComponent* meshComp = new MeshComponent(this, mesh);
	meshComp->SetTextureIndex(1); 
	RigidbodyComponent* rb = new RigidbodyComponent(this);
	BoxAABBComponent* bc = new BoxAABBComponent(this, 10, GetTransformComponent().GetSize());*/
}

void Cube::Update()
{
	Actor::Update();
}

void Cube::Destroy()
{
	Actor::Destroy();
}

void Cube::OnTriggerEnter(ColliderComponent* collider, HitResult* infos)
{
}

void Cube::OnTriggerStay(ColliderComponent* collider, HitResult* infos)
{
}

void Cube::OnTriggerExit(ColliderComponent* collider, HitResult* infos)
{
}
