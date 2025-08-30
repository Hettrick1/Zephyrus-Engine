#pragma once
#include "Panel.h"

class ConsolePanel : public Panel
{
private:

public:
	ConsolePanel(const std::string& pName);
	~ConsolePanel();
	void Draw() override;
};
