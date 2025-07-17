#include "EmptyActor.h"
#include "SceneManager.h"

EmptyActor::EmptyActor(Vector3D pPos, Vector3D pSize, Quaternion pRotation)
	: Actor(pPos, pSize, pRotation)
{
	Start();
	mScene.AddActor(this);
}

EmptyActor::~EmptyActor()
{
}

void EmptyActor::Start()
{
	Actor::Start();
}

void EmptyActor::Update()
{
	Actor::Update();
}

void EmptyActor::Destroy()
{
	Actor::Destroy();
}
