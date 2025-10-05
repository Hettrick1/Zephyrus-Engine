#pragma once
#include "Panel.h"
#include "Log.h"
#include "ILogListener.h"
#include <deque>

class ConsolePanel : public Panel, public Zephyrus::Debug::ILogListener
{
private:
	std::deque<Zephyrus::Debug::ZPMessage> mLogMessages;
	size_t mMessageQuantity = 0;
	size_t mMaxMessageInConsole = 150;
	bool mNewMessage = false;

	bool mShowInfos = true;
	bool mShowLoads = false;
	bool mShowWarns = true;
	bool mShowErrors = true;

public:
	ConsolePanel(ISceneContext* pSceneContext, const std::string& pName);
	~ConsolePanel();
	void Update() override;
	void Draw() override;
	void OnLogMessage(const Zephyrus::Debug::ZPMessage& pMessage) override;
};
