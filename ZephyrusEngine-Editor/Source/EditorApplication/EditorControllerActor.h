#pragma once
#include "Actor.h"

class EditorControllerActor : public Actor
{
private:
	
public:
	EditorControllerActor(const Vector3D& pPos = 0, const Vector3D& pSize = 1, const Quaternion& pRotation = Quaternion(0, 0));
	~EditorControllerActor();
	void Start() override;
	void Update() override;
	void Destroy() override;
};