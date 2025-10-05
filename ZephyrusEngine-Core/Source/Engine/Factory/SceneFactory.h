#pragma once
#include "Scene.h"

namespace Zephyrus::Scenes {
	class Scene;
}

class ISceneContext;

using Zephyrus::Scenes::Scene;

namespace Zephyrus::Factory {
	class SceneFactory
	{
	private:
		ISceneContext* mSceneContext{ nullptr };
	public:
		SceneFactory(ISceneContext* pSceneContext);
		~SceneFactory() = default;
		bool PopulateSceneFromFile(Scene* pSceneRef, const std::string& pFilePath);
	};
}