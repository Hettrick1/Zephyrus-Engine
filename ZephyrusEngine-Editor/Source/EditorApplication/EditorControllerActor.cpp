#include "pch.h"
#include "EditorControllerActor.h"
#include "SceneManager.h"
#include "CameraComponent.h"
#include "EditorControllerComponent.h"

namespace Zephyrus::ActorComponent {
	EditorControllerActor::EditorControllerActor(ISceneContext* pContext, Scene& pScene, const Vector3D& pPos, const Vector3D& pSize, const Quaternion& pRotation)
		: Actor(pContext, pScene, pPos, pSize, pRotation)
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