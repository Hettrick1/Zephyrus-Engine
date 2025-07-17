#pragma once
#include "Actor.h"

class EmptyActor : public Actor
{
public:
	EmptyActor(Vector3D pPos = 0, Vector3D pSize = 1, Quaternion pRotation = Quaternion(0, 0));
	~EmptyActor();
	void Start() override;
	void Update() override;
	void Destroy() override;
};