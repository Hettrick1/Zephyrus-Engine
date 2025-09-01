#pragma once
#include "Panel.h"
#include "Log.h"
#include "ILogListener.h"
#include <deque>

class ConsolePanel : public Panel, public Zephyrus::ILogListener
{
private:
	std::deque<Zephyrus::ZPMessage> mLogMessages;
	size_t mMessageQuantity = 0;
	size_t mMaxMessageInConsole = 50;
	bool mNewMessage = false;
public:
	ConsolePanel(const std::string& pName);
	~ConsolePanel();
	void Update() override;
	void Draw() override;
	void OnLogMessage(const Zephyrus::ZPMessage& pMessage) override;
};
