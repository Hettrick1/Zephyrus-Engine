#pragma once
#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"

#include <string>

class Panel
{
protected:
	std::string mName;
	bool mDrawPanel = true;
public:
	Panel(const std::string& pName);
	virtual ~Panel();

	virtual void Update();

	virtual void BeginDraw();
	virtual void Draw() = 0;
	virtual void EndDraw();

	inline bool GetDrawPanel() const { return mDrawPanel; }
	inline void SetDrawPanel(bool pDrawPanel) { mDrawPanel = pDrawPanel; }
};

