#pragma once
#include "Panel.h"
#include "SDL_opengl.h"

class ScenePanel : public Panel
{
private:
	GLuint mSceneRenderTexture = 0;
	bool mIsHover = false;
public:
	ScenePanel(const std::string& pName, GLuint pSceneRenderTexture);
	~ScenePanel();
	void Draw() override;
	inline bool GetIsHover() const { return mIsHover; }
};
