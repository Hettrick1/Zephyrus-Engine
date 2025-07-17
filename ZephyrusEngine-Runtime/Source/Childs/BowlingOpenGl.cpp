#include "BowlingOpenGl.h"
#include "Game/TestOpenGl/testGlPlayer.h"
#include "Game/TestOpenGl/Cube.h"
#include "Game/TestOpenGl/BowlingBall.h"
#include "Game/TestOpenGl/BowlingPin.h"
#include "CameraActor.h"
#include "Game/TestOpenGl/BowlingPlayer.h"
#include "RigidbodyComponent.h"
#include "BoxAABBComponent.h"
#include "MeshComponent.h"
#include "CoreActors/BasicSATCube.h"
#include "CoreActors/BasicCube.h"
#include "DebugRenderer.h"
#include "HudElements/HudText.h"
#include "HudManager.h"

BowlingOpenGl::BowlingOpenGl()
	: mBowlingPlayer(nullptr)
{
}

BowlingOpenGl::~BowlingOpenGl()
{
}

void BowlingOpenGl::Start(IRenderer* renderer)
{
	Scene::Start(renderer);
	GetRenderer()->GetDebugRenderer()->SetDrawDebug(false);

	mBowlingPlayer = new BowlingPlayer();
	mBowlingPlayer->GetTransformComponent().RotateX(-5);
	mBowlingPlayer->SetPosition(Vector3D(0.0f, -40.0f, 5.0f));
	AddActor(mBowlingPlayer);
	mBowlingPlayer->Start();

	BasicCube* ground = new BasicCube();
	AddActor(ground);
	ground->SetPosition(Vector3D(0.0, 35.0f, -6.0f));
	ground->GetTransformComponent().SetSize({ 15, 60, 1 });
	ground->Start();
	ground->GetComponentOfType<MeshComponent>()->SetTiling(Vector2D(2, 1));
	ground->GetComponentOfType<MeshComponent>()->SetTextureIndex(0);

	BasicSATCube* wall = new BasicSATCube();
	AddActor(wall);
	wall->SetPosition(Vector3D(15.0f, 35.0f, 0.0f));
	wall->GetTransformComponent().SetSize({ 1, 60, 15 });
	wall->AddComponent(new RigidbodyComponent(wall));
	wall->GetRigidBody()->SetMass(10000000);
	wall->Start();
	wall->GetComponentOfType<MeshComponent>()->SetTextureIndex(6);

	BasicSATCube* wall1 = new BasicSATCube();
	AddActor(wall1);
	wall1->SetPosition(Vector3D(-15.0f, 35.0f, 0.0f));
	wall1->GetTransformComponent().SetSize({ 1, 60, 15 });
	wall1->AddComponent(new RigidbodyComponent(wall1));
	wall1->GetRigidBody()->SetMass(10000000);
	wall1->Start();
	wall1->GetComponentOfType<MeshComponent>()->SetTextureIndex(6);

	BasicSATCube* wall2 = new BasicSATCube();
	AddActor(wall2);
	wall2->SetPosition(Vector3D(0.0f, 80.0f, 0.0f));
	wall2->GetTransformComponent().SetSize({ 15, 1, 15 });
	wall2->AddComponent(new RigidbodyComponent(wall2));
	wall2->GetRigidBody()->SetMass(10000000);
	wall2->Start();
	wall2->GetComponentOfType<MeshComponent>()->SetTextureIndex(6);

	CameraActor* camera = new CameraActor();
	camera->SetPosition(Vector3D(0.0f, 34.0f, 40.0f));
	camera->GetTransformComponent().RotateX(-90);
	AddActor(camera);
	camera->Start();

	HudText* text = new HudText("Tab - Change view", -1800, 900, 0.5f, Vector4D(0.5, 0.5, 6.0, 1.0), TextAlignment::LEFT);
	renderer->GetHud()->AddElement(text);
	HudText* text2 = new HudText("Space - Shoot", -1800, 800, 0.5f, Vector4D(0.5, 0.5, 6.0, 1.0), TextAlignment::LEFT);
	renderer->GetHud()->AddElement(text2);
	HudText* text3 = new HudText("Q/D - Move", -1800, 700, 0.5f, Vector4D(0.5, 0.5, 6.0, 1.0), TextAlignment::LEFT);
	renderer->GetHud()->AddElement(text3);
}

void BowlingOpenGl::Update()
{
	Scene::Update();
}

void BowlingOpenGl::Render()
{
	Scene::Render();
}

void BowlingOpenGl::Close()
{
}
