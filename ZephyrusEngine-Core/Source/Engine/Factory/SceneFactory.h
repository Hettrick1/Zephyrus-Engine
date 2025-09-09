#pragma once
#include "Scene.h"

class SceneFactory
{
public:
	static bool PopulateSceneFromFile(const std::string& pFilePath);
};