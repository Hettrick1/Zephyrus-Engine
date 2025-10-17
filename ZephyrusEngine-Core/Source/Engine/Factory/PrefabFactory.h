#pragma once
#include "IPrefabFactory.h"

namespace Zephyrus::Factory {
	class PrefabFactory : public IPrefabFactory
	{
	private:
		ComponentFactory* mComponentFactory{nullptr};
		ISceneContext* mSceneContext{ nullptr };
	public:
		explicit PrefabFactory(ISceneContext* pSceneContext, ComponentFactory* pComponentFactory);
		~PrefabFactory() = default;

		Actor* SpawnActorFromPrefab(Scene* pScene, const std::string& pPrefabName, const Vector3D& pInitialPos = 0, const Vector3D& pInitialRot = 0, const Vector3D& pInitialSize = 1) override;
		Actor* InitPrefab(Scene* pScene, const std::string& pPrefabName) override;
		std::vector<std::string> GetPrefabFiles(const std::string& folderPath) override;
		Component* CreateAndAttachComponent(Serialization::IDeserializer& reader, Actor* actor, bool doDeserialize = true) override;
	};
}