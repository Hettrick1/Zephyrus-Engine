#include "LVLDoom.h"
#include "Temp/DoomComponents/DoomPC.h"
#include "CameraActor.h"
#include "MeshComponent.h"
#include "CoreActors/BasicCube.h"
#include "CoreActors/SkySphere.h"
#include "FactoryJSON/PrefabFactory.h"
#include "DebugRenderer.h"
#include "BoxAABBComponent.h"

LVLDoom::LVLDoom()
{
}

LVLDoom::~LVLDoom()
{
}

void LVLDoom::Start(IRenderer* renderer)
{
	Scene::Start(renderer);
	
	GetRenderer()->GetDebugRenderer()->SetDrawDebug(true);
	/*mPlayer = new DoomPlayer();
	mPlayer->SetPosition(Vector3D(-48, 0, 0));
	mPlayer->RotateZ(-90);*/

	PrefabFactory::CreateActorFromPrefab("Doom/DoomPlayer");

	//PrefabFactory::CreateActorFromPrefab("CameraActor");
	auto actor = PrefabFactory::CreateActorFromPrefab("SkyBoxActor");
	CubeTextureMap cubemap;
	cubemap.CreateCubeTextureMap({
		"Sprites/Doom/skybox/front.png",
		"Sprites/Doom/skybox/back.png",
		"Sprites/Doom/skybox/top.png",
		"Sprites/Doom/skybox/bottom.png",
		"Sprites/Doom/skybox/left.png",
		"Sprites/Doom/skybox/right.png",
		});
	actor->GetComponentOfType<SkySphereComponent>()->SetTextureIndex(cubemap.GetID());

	auto wall = PrefabFactory::CreateActorFromPrefab("CubeActor");
	wall->SetPosition(Vector3D(0, 50, -0.21));
	wall->SetSize(Vector3D(50, 1, 1));
	wall->GetComponentOfType<BoxAABBComponent>()->SetSize(Vector3D(50, 1, 1));
	wall->RotateX(90);
	wall->GetComponentOfType<MeshComponent>()->SetTextureIndex(3);
	wall->GetComponentOfType<MeshComponent>()->SetTiling(Vector2D(150, 6));
	wall->SetTag("Wall");

	auto wall1 = PrefabFactory::CreateActorFromPrefab("CubeActor");
	wall1->SetPosition(Vector3D(0, -50, -0.21));
	wall1->SetSize(Vector3D(50, 1, 1));
	wall1->GetComponentOfType<BoxAABBComponent>()->SetSize(Vector3D(50, 1, 1));
	wall1->RotateX(90);
	wall1->GetComponentOfType<MeshComponent>()->SetTextureIndex(3);
	wall1->GetComponentOfType<MeshComponent>()->SetTiling(Vector2D(150, 6));
	wall1->SetTag("Wall");

	auto wall2 = PrefabFactory::CreateActorFromPrefab("CubeActor");
	wall2->SetPosition(Vector3D(50, 0, -0.21));
	wall2->SetSize(Vector3D(1, 50, 1));
	wall2->GetComponentOfType<BoxAABBComponent>()->SetSize(Vector3D(1, 50, 1));
	wall2->RotateY(90);
	wall2->GetComponentOfType<MeshComponent>()->SetTiling(Vector2D(6, 150));
	wall2->SetTag("Wall");

	auto wall3 = PrefabFactory::CreateActorFromPrefab("CubeActor");
	wall3->SetPosition(Vector3D(-50, 0, -0.21));
	wall3->SetSize(Vector3D(1, 50, 1));
	wall3->GetComponentOfType<BoxAABBComponent>()->SetSize(Vector3D(1, 50, 1));
	wall3->RotateY(90);
	wall3->GetComponentOfType<MeshComponent>()->SetTiling(Vector2D(6, 150));
	wall3->SetTag("Wall");

	auto wall4 = PrefabFactory::CreateActorFromPrefab("CubeActor");
	wall4->SetPosition(Vector3D(0, -30, -0.21));
	wall4->SetSize(Vector3D(1, 20, 1));
	wall4->GetComponentOfType<BoxAABBComponent>()->SetSize(Vector3D(1, 20, 1));
	wall4->RotateY(90);
	wall4->GetComponentOfType<MeshComponent>()->SetTiling(Vector2D(6, 150));
	wall4->SetTag("Wall");

	auto wall5 = PrefabFactory::CreateActorFromPrefab("CubeActor");
	wall5->SetPosition(Vector3D(0, 30, -0.21));
	wall5->SetSize(Vector3D(1, 20, 1));
	wall5->GetComponentOfType<BoxAABBComponent>()->SetSize(Vector3D(1, 20, 1));
	wall5->RotateY(90);
	wall5->GetComponentOfType<MeshComponent>()->SetTiling(Vector2D(6, 150));
	wall5->SetTag("Wall");

	auto wall6 = PrefabFactory::CreateActorFromPrefab("CubeActor");
	wall6->SetPosition(Vector3D(0, -9, -0.21));
	wall6->SetSize(Vector3D(1, 1, 1));
	wall6->GetComponentOfType<BoxAABBComponent>()->SetSize(Vector3D(1, 1, 1));
	wall6->RotateY(90);
	wall6->GetComponentOfType<MeshComponent>()->SetTiling(Vector2D(6, 6));
	wall6->SetTag("Wall");

	auto wall7 = PrefabFactory::CreateActorFromPrefab("CubeActor");
	wall7->SetPosition(Vector3D(0, 9, -0.21));
	wall7->SetSize(Vector3D(1, 1, 1));
	wall7->GetComponentOfType<BoxAABBComponent>()->SetSize(Vector3D(1, 1, 1));
	wall7->RotateY(90);
	wall7->GetComponentOfType<MeshComponent>()->SetTiling(Vector2D(6, 6));
	wall7->SetTag("Wall");

	auto wall8 = PrefabFactory::CreateActorFromPrefab("CubeActor");
	wall8->SetPosition(Vector3D(0, 0, -1.2));
	wall8->SetSize(Vector3D(50, 50, 0.1));
	wall8->GetComponentOfType<BoxAABBComponent>()->SetSize(Vector3D(50, 50, 0.1));
	wall8->GetComponentOfType<MeshComponent>()->SetTextureIndex(2);
	wall8->GetComponentOfType<MeshComponent>()->SetTiling(Vector2D(300, 300));
	wall8->SetTag("Wall");

	auto wall9 = PrefabFactory::CreateActorFromPrefab("CubeActor");
	wall9->SetPosition(Vector3D(-30, 0, -0.21));
	wall9->SetSize(Vector3D(1, 1, 1));
	wall9->GetComponentOfType<BoxAABBComponent>()->SetSize(Vector3D(1, 1, 1));
	wall9->RotateY(0);
	wall9->GetComponentOfType<MeshComponent>()->SetTiling(Vector2D(6, 6));
	wall9->SetTag("Wall");

	auto wall10 = PrefabFactory::CreateActorFromPrefab("CubeActor");
	wall10->SetPosition(Vector3D(-48, -10, -0.21));
	wall10->SetSize(Vector3D(1, 1, 1));
	wall10->GetComponentOfType<BoxAABBComponent>()->SetSize(Vector3D(1, 1, 1));
	wall10->RotateY(0);
	wall10->GetComponentOfType<MeshComponent>()->SetTiling(Vector2D(6, 6));
	wall10->SetTag("Wall");

	auto wall11 = PrefabFactory::CreateActorFromPrefab("CubeActor");
	wall11->SetPosition(Vector3D(-48, 10, -0.21));
	wall11->SetSize(Vector3D(1, 1, 1));
	wall11->GetComponentOfType<BoxAABBComponent>()->SetSize(Vector3D(1, 1, 1));
	wall11->RotateY(0);
	wall11->GetComponentOfType<MeshComponent>()->SetTiling(Vector2D(6, 6));
	wall11->SetTag("Wall");

	auto amoPu = PrefabFactory::CreateActorFromPrefab("Doom/AmoPickUp");
	amoPu->SetPosition(Vector3D(0, 0, -0.5));

	auto HealthPu = PrefabFactory::CreateActorFromPrefab("Doom/HealthPickUp");
	HealthPu->SetPosition(Vector3D(-20, -30, -0.5));
	auto HealthPu2 = PrefabFactory::CreateActorFromPrefab("Doom/HealthPickUp");
	HealthPu2->SetPosition(Vector3D(-20, 30, -0.5));

	auto shieldPu = PrefabFactory::CreateActorFromPrefab("Doom/ShieldPickUp");
	shieldPu->SetPosition(Vector3D(-40, -40, -0.5));
	auto shieldPu2 = PrefabFactory::CreateActorFromPrefab("Doom/ShieldPickUp");
	shieldPu2->SetPosition(Vector3D(-40, 40, -0.5));

	/*
	DoomEnemy* enemy = new DoomEnemy(mPlayer, Vector3D(-10, -20, -0.1));
	DoomEnemy* enemy2 = new DoomEnemy(mPlayer, Vector3D(-10, 20, -0.1));
	DoomEnemy* enemy3 = new DoomEnemy(mPlayer, Vector3D(-10, 0, -0.1));
	DoomEnemy* enemy4 = new DoomEnemy(mPlayer, Vector3D(-45, -45, -0.1));
	DoomEnemy* enemy5 = new DoomEnemy(mPlayer, Vector3D(-45, 45, -0.1));*/
}

void LVLDoom::Update()
{
	Scene::Update();
}

void LVLDoom::Render()
{
	Scene::Render();
}

void LVLDoom::Close()
{
	Scene::Close();
}
