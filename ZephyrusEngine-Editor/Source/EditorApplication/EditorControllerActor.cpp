#include "EditorControllerActor.h"
#include "SceneManager.h"
#include "CameraComponent.h"
#include "EditorControllerComponent.h"

namespace Zephyrus::ActorComponent {
	EditorControllerActor::EditorControllerActor(Scene& pScene, const Vector3D& pPos, const Vector3D& pSize, const Quaternion& pRotation)
		: Actor(pScene, pPos, pSize, pRotation)
	{
		auto camera = new CameraComponent(this, 1920, 1080, CameraUsage::Editor);
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
	{
		Actor::Destroy();
	}
}