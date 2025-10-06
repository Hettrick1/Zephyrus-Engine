#pragma once
#include "Actor.h"
#include "ComponentFactory.h"
#include <string>

namespace Zephyrus::Debug
{
	class Log;
}
namespace Zephyrus::Scenes
{
	class Scene;
}

using Zephyrus::ActorComponent::Actor;
using Zephyrus::ActorComponent::Component;
using Zephyrus::Debug::Log;

namespace Zephyrus::Factory {
	class PrefabFactory
	{
	private:
		ComponentFactory* mComponentFactory{nullptr};
		ISceneContext* mSceneContext{ nullptr };
	public:
		explicit PrefabFactory(ISceneContext* pSceneContext, ComponentFactory* pComponentFactory);
		~PrefabFactory() = default;

		Actor* SpawnActorFromPrefab(Scene* pScene, const std::string& pPrefabName, const Vector3D& pInitialPos = 0, const Vector3D& pInitialRot = 0, const Vector3D& pInitialSize = 1);
		Actor* InitPrefab(Scene* pScene, const std::string& pPrefabName);
		std::vector<std::string> GetPrefabFiles(const std::string& folderPath);
		Component* CreateAndAttachComponent(Serialization::IDeserializer& reader, Actor* actor, bool doDeserialize = true);
	};
}