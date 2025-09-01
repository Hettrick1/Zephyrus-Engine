#pragma once
#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"

#include <string>

class Panel
{
protected:
	std::string mName;
public:
	Panel(const std::string& pName);
	virtual ~Panel();

	virtual void Update();

	virtual void BeginDraw();
	virtual void Draw() = 0;
	virtual void EndDraw();
};

