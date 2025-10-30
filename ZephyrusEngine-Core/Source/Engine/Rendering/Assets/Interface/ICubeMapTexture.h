#pragma once
#include <vector>

#include "ITextureBase.h"

namespace Zephyrus::Assets
{
	class ICubeMapTexture : public ITextureBase
	{
	public:
		virtual ~ICubeMapTexture() = default;

		virtual bool Load(const std::vector<std::string>& pCubePaths) = 0;
		virtual inline std::vector<std::string> GetFaceFilePath() const = 0;
		virtual void SetTempFilePath(const std::vector<std::string>& pFilePath) = 0;
		virtual std::vector<std::string> GetTempFilePath() const = 0;
	};

}