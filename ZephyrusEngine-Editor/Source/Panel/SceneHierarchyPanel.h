#pragma once
#include "Panel.h"

namespace Zephyrus::ActorComponent
{
	class Actor;
}


class SceneHierarchyPanel : public Panel
{
private:
	Zephyrus::ActorComponent::Actor* mSelectedActor = nullptr;
public:
	SceneHierarchyPanel(const std::string& pName);
	~SceneHierarchyPanel();
	void Draw() override;
	void ResetSelectedActor();
	Zephyrus::ActorComponent::Actor* GetSelectedActor() const;
};
