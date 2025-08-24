#pragma once
#include "Scene.h"

class LVLDoom : public Scene
{
public:
	LVLDoom();
	~LVLDoom();
	void Start(IRenderer* renderer) override;
	void Update() override;
	void Render() override;
	void Close() override;
};


