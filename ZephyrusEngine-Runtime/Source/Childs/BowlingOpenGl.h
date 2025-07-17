#pragma once

#include "Scene.h"
#include "Shaders/ShaderProgram.h"

class BowlingPlayer;

class BowlingOpenGl : public Scene
{
public:
	BowlingOpenGl();
	~BowlingOpenGl();
	void Start(IRenderer* renderer) override;
	void Update() override;
	void Render() override;
	void Close() override;
private:
	BowlingPlayer* mBowlingPlayer;
};

