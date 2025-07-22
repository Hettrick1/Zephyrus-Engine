#include "BasicSATCube.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "Assets.h"
#include "Scene.h"
#include "BoxSATComponent.h"

BasicSATCube::BasicSATCube(Vector3D pPos, Vector3D pSize, Quaternion pRotation, ShaderProgram * pProgram)
	: Actor(pPos, pSize, pRotation), mShaderProgram(pProgram)
{
}

BasicSATCube::~BasicSATCube()
{
}

void BasicSATCube::Start()
{
	Actor::Start();
	Texture* tex1 = Assets::LoadTexture(*GetScene().GetRenderer(), "../Imports/Sprites/Doom/268.png", "wallTex");
	Texture* tex2 = Assets::LoadTexture(*GetScene().GetRenderer(), "../Imports/Sprites/Doom/091.png", "groundTex");
	Texture* tex3 = Assets::LoadTexture(*GetScene().GetRenderer(), "../Imports/Sprites/Doom/268bis.png", "wallTex2");
	Texture* tex4 = Assets::LoadTexture(*GetScene().GetRenderer(), "../Imports/Sprites/uv_mapper.jpg", "uvMapper");
	Texture* tex5 = Assets::LoadTexture(*GetScene().GetRenderer(), "../Imports/Sprites/gray.png", "wallDefault");
	Mesh* mesh = Assets::LoadMesh("../Imports/Meshes/cube.obj", "cube");
	mesh->AddTexture(tex1);
	mesh->AddTexture(tex2);
	mesh->AddTexture(tex3);
	mesh->AddTexture(tex4);
	mesh->AddTexture(tex5);
	MeshComponent* meshComp = new MeshComponent(this, mesh, mShaderProgram);
	meshComp->SetTextureIndex(1);
	BoxSATComponent* bc = new BoxSATComponent(this, 10, GetTransformComponent().GetSize());
}

void BasicSATCube::Update()
{
	Actor::Update();
}

void BasicSATCube::Destroy()
{
	Actor::Destroy();
}

void BasicSATCube::OnTriggerEnter(ColliderComponent* pCollider, HitResult* pInfos)
{
}

void BasicSATCube::OnTriggerStay(ColliderComponent* pCollider, HitResult* pInfos)
{
}

void BasicSATCube::OnTriggerExit(ColliderComponent* pCollider, HitResult* pInfos)
{
}
