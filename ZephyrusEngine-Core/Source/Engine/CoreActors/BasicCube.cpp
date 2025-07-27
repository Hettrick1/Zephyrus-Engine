#include "BasicCube.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "Assets.h"
#include "Scene.h"
#include "BoxAABBComponent.h"

BasicCube::BasicCube(const Vector3D& pPos, const Vector3D& pSize, const Quaternion& pRotation, ShaderProgram* pProgram)
	: Actor(pPos, pSize, pRotation), mShaderProgram(pProgram)
{
	Start();
	mScene.AddActor(this);
}

BasicCube::~BasicCube()
{
}

void BasicCube::Start()
{
	Actor::Start();
	Texture* tex1 = Assets::LoadTexture("../Imports/Sprites/Doom/268.png", "wallTex");
	Texture* tex2 = Assets::LoadTexture("../Imports/Sprites/Doom/091.png", "groundTex");
	Texture* tex3 = Assets::LoadTexture("../Imports/Sprites/Doom/268bis.png", "wallTex2");
	Texture* tex4 = Assets::LoadTexture("../Imports/Sprites/uv_mapper.jpg", "uvMapper");
	Mesh* mesh = Assets::LoadMesh("../Imports/Meshes/cube.obj", "cube");
	mesh->AddTexture(tex1);
	mesh->AddTexture(tex2); 
	mesh->AddTexture(tex3); 
	mesh->AddTexture(tex4);
	MeshComponent* meshComp = new MeshComponent(this, mesh, mShaderProgram);
	meshComp->SetTextureIndex(1);
	BoxAABBComponent* bc = new BoxAABBComponent(this, 10, GetTransformComponent().GetSize());
}

void BasicCube::Update()
{
	Actor::Update();
}

void BasicCube::Destroy()
{
	Actor::Destroy();
}
