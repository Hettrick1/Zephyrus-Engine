#pragma once
#include "Panel.h"
#include "SDL_opengl.h"
#include "Vector2D.h"

class ScenePanel : public Panel
{
private:
	GLuint mSceneRenderTexture = 0;
	bool mIsHover = false;
	Vector2D mDimensions;
public:
	ScenePanel(ISceneContext* pSceneContext, const std::string& pName, GLuint pSceneRenderTexture);
	~ScenePanel();
	void Draw() override;
	inline Vector2D GetDimensions() const { return mDimensions; }
	inline bool GetIsHover() const { return mIsHover; }
};
