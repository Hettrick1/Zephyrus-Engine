#pragma once
#include "Scene.h"

class SceneFactory
{
public:
	static bool CreateSceneFromFile(const std::string& pFilePath);
};