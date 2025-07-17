#pragma once
#include "Actor.h"

class BallSpawnPoint : public Actor
{
public:
	BallSpawnPoint(Vector3D pPos = 0, Vector3D pSize = 1, Quaternion pRotation = Quaternion(0, 0));
	~BallSpawnPoint();
	void Start() override;
	void Update() override;
	void Destroy() override;
private:
};

