#pragma once

#include "Actor.h"

class CameraActor : public Actor
{
public:
	CameraActor();
	~CameraActor();
	void Start() override;
	void Update() override;
	void Destroy() override;
private:

};

