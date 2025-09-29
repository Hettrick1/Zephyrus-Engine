#include "EditorControllerActor.h"
#include "SceneManager.h"
#include "CameraComponent.h"
#include "NewCameraComponent.h"
#include "EditorControllerComponent.h"
#include "CameraManager.h"

EditorControllerActor::EditorControllerActor(const Vector3D& pPos, const Vector3D& pSize, const Quaternion& pRotation)
	: Actor(pPos, pSize, pRotation)
{
	auto camera = new NewCameraComponent(this, 1920, 1080, CameraUsage::Editor);
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
	CameraManager::Instance().SetCurrentCamera(GetComponentOfType<CameraComponent>());
	//SceneManager::ActiveScene->GetCameraManager()->AddCamera(GetComponentOfType<NewCameraComponent>());
}

void EditorControllerActor::Update()
{
}

void EditorControllerActor::Destroy()
{;
	Actor::Destroy();
}
