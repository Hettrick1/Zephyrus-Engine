#pragma once
#include "Panel.h"
#include "SelectedActorAccessor.h"

namespace Zephyrus::ActorComponent
{
	class Actor;
}


class SceneHierarchyPanel : public Panel, public SelectedActorAccessor
{
private:
	Zephyrus::ActorComponent::Actor* mSelectedActor = nullptr;
public:
	SceneHierarchyPanel(ISceneContext* pSceneContext, const std::string& pName);
	~SceneHierarchyPanel() override;
	void Draw() override;
	void ResetSelectedActor();
	Zephyrus::ActorComponent::Actor* GetSelectedActor() const override;
};
