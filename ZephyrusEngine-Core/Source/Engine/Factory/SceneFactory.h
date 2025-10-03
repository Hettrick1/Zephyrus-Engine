#pragma once
#include "Scene.h"

namespace Zephyrus::Factory {
	class SceneFactory
	{
	public:
		static bool PopulateSceneFromFile(const std::string& pFilePath);
	};
}