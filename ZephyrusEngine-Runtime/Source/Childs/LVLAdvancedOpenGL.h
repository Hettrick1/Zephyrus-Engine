#pragma once

#include "Scene.h"
#include "Shaders/ShaderProgram.h"

class LVLAdvancedOpenGl : public Scene
{
public:
	LVLAdvancedOpenGl();
	~LVLAdvancedOpenGl();
	void Start(IRenderer* renderer) override;
	void Update() override;
	void Render() override;
	void Close() override;
private:
};
