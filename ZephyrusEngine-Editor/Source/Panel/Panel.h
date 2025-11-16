#pragma once
#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"
#include "ISceneContext.h"

#include <string>

class Panel
{
protected:
	std::string mName{ "" };
	bool mDrawPanel{ true };
	ISceneContext* mContext{ nullptr };
public:
	Panel(ISceneContext* pSceneContext, const std::string& pName);
	virtual ~Panel() = default;

	virtual void Update();

	virtual void BeginDraw();
	virtual void Draw() = 0;
	virtual void EndDraw();

	inline bool GetDrawPanel() const { return mDrawPanel; }
	inline void SetDrawPanel(bool pDrawPanel) { mDrawPanel = pDrawPanel; }
};

