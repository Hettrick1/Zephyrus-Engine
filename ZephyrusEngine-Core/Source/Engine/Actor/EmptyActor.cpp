#include "EmptyActor.h"
#include "SceneManager.h"

namespace Zephyrus::ActorComponent
{
	EmptyActor::EmptyActor(const Vector3D& pPos, const Vector3D& pSize, const Quaternion& pRotation)
		: Actor(pPos, pSize, pRotation)
	{
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
}