#include "EditorControllerActor.h"
#include "SceneManager.h"
#include "CameraComponent.h"
#include "EditorControllerComponent.h"

EditorControllerActor::EditorControllerActor(const Vector3D& pPos, const Vector3D& pSize, const Quaternion& pRotation)
	: Actor(pPos, pSize, pRotation)
{
	auto camera = new CameraComponent(this);
	AddComponent(camera);

	auto editorController = new EditorControllerComponent(this);
	AddComponent(editorController);
}

EditorControllerActor::~EditorControllerActor()
{
}

void EditorControllerActor::Start()
{
	Actor::Start();
}

void EditorControllerActor::Update()
{
}

void EditorControllerActor::Destroy()
{;
	Actor::Destroy();
}
