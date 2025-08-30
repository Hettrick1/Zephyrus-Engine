#pragma once
#include "Panel.h"
#include "SDL_opengl.h"

class ScenePanel : public Panel
{
private:
	GLuint mSceneRenderTexture = 0;
public:
	ScenePanel(const std::string& pName, GLuint pSceneRenderTexture);
	~ScenePanel();
	void Draw() override;
};
