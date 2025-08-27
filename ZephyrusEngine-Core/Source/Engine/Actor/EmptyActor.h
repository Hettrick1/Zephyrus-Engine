#pragma once
#include "Actor.h"

class EmptyActor : public Actor
{
public:
	EmptyActor(const Vector3D& pPos = 0, const Vector3D& pSize = 1, const Quaternion& pRotation = Quaternion(0, 0));
	~EmptyActor();
	void Start() override;
	void Update() override;
	void Destroy() override;
};