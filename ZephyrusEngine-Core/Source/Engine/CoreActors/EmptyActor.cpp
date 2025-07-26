#include "EmptyActor.h"
#include "SceneManager.h"

EmptyActor::EmptyActor(const Vector3D& pPos, const Vector3D& pSize, const Quaternion& pRotation)
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
