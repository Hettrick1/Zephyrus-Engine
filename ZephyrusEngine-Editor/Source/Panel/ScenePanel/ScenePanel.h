#pragma once
#include "../Panel.h"
#include "Vector2D.h"
#include "GuizmoSettingsBar.h"
#include "Matrix4DRow.h"

class SelectedActorAccessor;

namespace Zephyrus::ActorComponent
{
	class Actor;
}

class ScenePanel : public Panel
{
private:
	unsigned int  mSceneRenderTexture = 0;
	bool mIsHover = false;
	Vector2D mDimensions;
	GuizmoSettingsBar mSettingsBar;

	Matrix4DRow cameraView;
	Matrix4DRow cameraProjection;

	SelectedActorAccessor* mSelectedActorAccessor = nullptr;

	bool mDeactivatedAfterEdit = false;
	Matrix4DRow oldTransform = Matrix4DRow::Identity;
	
public:
	ScenePanel(ISceneContext* pSceneContext, const std::string& pName, unsigned int pSceneRenderTexture, SelectedActorAccessor* actorAccessor);
	~ScenePanel() override;
	void Draw() override;

	void UpdateMatrices(Matrix4DRow view, Matrix4DRow projection)
	{
		cameraView = view;
		cameraProjection = projection;
	}
	
	inline Vector2D GetDimensions() const { return mDimensions; }
	inline bool GetIsHover() const { return mIsHover; }
};
