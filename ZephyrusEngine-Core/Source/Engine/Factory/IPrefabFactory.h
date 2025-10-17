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

	class IPrefabFactory
	{
	public:
		~IPrefabFactory() = default;

		virtual Actor* SpawnActorFromPrefab(Scene* pScene, const std::string& pPrefabName, const Vector3D& pInitialPos = 0, const Vector3D& pInitialRot = 0, const Vector3D& pInitialSize = 1) = 0;
		virtual Actor* InitPrefab(Scene* pScene, const std::string& pPrefabName) = 0;
		virtual std::vector<std::string> GetPrefabFiles(const std::string& folderPath) = 0;
		virtual Component* CreateAndAttachComponent(Serialization::IDeserializer& reader, Actor* actor, bool doDeserialize = true) = 0;
	};
}