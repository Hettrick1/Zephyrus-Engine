#pragma once
#include "Panel.h"
#include "Vector2D.h"

class ScenePanel : public Panel
{
private:
	unsigned int  mSceneRenderTexture = 0;
	bool mIsHover = false;
	Vector2D mDimensions;
public:
	ScenePanel(ISceneContext* pSceneContext, const std::string& pName, unsigned int pSceneRenderTexture);
	~ScenePanel() override;
	void Draw() override;
	inline Vector2D GetDimensions() const { return mDimensions; }
	inline bool GetIsHover() const { return mIsHover; }
};
