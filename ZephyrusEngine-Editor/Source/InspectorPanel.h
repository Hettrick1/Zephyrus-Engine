#pragma once
#include "Panel.h"

class InspectorPanel : public Panel
{
private:

public:
	InspectorPanel(const std::string& pName);
	~InspectorPanel();
	void Draw() override;
};
