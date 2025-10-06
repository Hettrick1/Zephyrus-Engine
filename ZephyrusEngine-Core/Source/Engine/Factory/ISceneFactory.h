#pragma once

#include "Scene.h"

namespace Zephyrus::Scenes {
	class Scene;
}

class ISceneContext;

using Zephyrus::Scenes::Scene;

namespace Zephyrus::Factory {
	class ISceneFactory
	{
	public:
		~ISceneFactory() = default;
		virtual bool PopulateSceneFromFile(Scene* pSceneRef, const std::string& pFilePath) = 0;
	};
}