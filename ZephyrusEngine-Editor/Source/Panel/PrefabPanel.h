#pragma once
#include "Panel.h"

class PrefabPanel : public Panel
{
private:
public:
	PrefabPanel(const std::string& pName);
	~PrefabPanel();
	void Draw() override;
};