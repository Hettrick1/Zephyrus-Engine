#pragma once
#include "Actor.h"

namespace Zephyrus::ActorComponent {
	class EditorControllerActor : public Actor
	{
	public:
		EditorControllerActor(ISceneContext* pContext, Scene& pScene, const Vector3D& pPos = 0, const Vector3D& pSize = 1, const Quaternion& pRotation = Quaternion(0, 0));
		~EditorControllerActor() override;
		void Start() override;
		void Update() override;
		void Destroy() override;
	};
}