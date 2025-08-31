#pragma once
#include "Panel.h"
#include "Log.h"
#include "ILogListener.h"

class ConsolePanel : public Panel, public Zephyrus::ILogListener
{
private:
	std::vector<Zephyrus::ZPMessage> mLogMessages;
public:
	ConsolePanel(const std::string& pName);
	~ConsolePanel();
	void Draw() override;
	void OnLogMessage(const Zephyrus::ZPMessage& pMessage) override;
};
