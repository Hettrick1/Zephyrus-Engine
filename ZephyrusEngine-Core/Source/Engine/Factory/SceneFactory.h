#pragma once
#include "Scene.h"

namespace Zephyrus::Scenes {
	class Scene;
}

using Zephyrus::Scenes::Scene;

namespace Zephyrus::Factory {
	class SceneFactory
	{
	private:
	public:
		SceneFactory() = default;
		~SceneFactory() = default;
		bool PopulateSceneFromFile(Scene* pSceneRef, const std::string& pFilePath);
	};
}