#pragma once
//#include "Scene.h"
//
//namespace Zephyrus::Scenes {
//	class Scene;
//}
//
//class ISceneContext;
//
//using Zephyrus::Scenes::Scene;

#include "ISceneFactory.h"

namespace Zephyrus::Factory {
	class SceneFactory : public ISceneFactory 
	{
	private:
		ISceneContext* mSceneContext{ nullptr };
	public:
		SceneFactory(ISceneContext* pSceneContext);
		~SceneFactory() = default;
		bool PopulateSceneFromFile(Scene* pSceneRef, const std::string& pFilePath) override;
	};
}