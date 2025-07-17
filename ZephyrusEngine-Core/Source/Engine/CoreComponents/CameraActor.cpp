#include "CameraActor.h"
#include "PlayerController3D.h"
#include "CameraComponent.h"
#include "SceneManager.h"

CameraActor::CameraActor()
	: Actor()
{
	Start();
	mScene.AddActor(this);
}

CameraActor::~CameraActor()
{
}

void CameraActor::Start()
{
	Actor::Start();
	CameraComponent* cc = new CameraComponent(this);
}

void CameraActor::Update()
{
	Actor::Update();
}

void CameraActor::Destroy()
{
	Actor::Destroy();
}
