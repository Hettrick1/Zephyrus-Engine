#pragma once
#include "Scene.h"

class LVLDoom : public Scene
{
private:
public:
	LVLDoom();
	~LVLDoom();
	void Start() override;
	void PostStart() override;
	void Update() override;
	void Render() override;
	void Close() override;
};


