#pragma once

#include <iostream>
#include "SDL.h"
#include "SDL_image.h"
#include "Vector2D.h"
#include "IRenderer.h"
#include <glew.h>

namespace Zephyrus::Render {
	class RendererOpenGl;
}
/**
 * @brief Manages a texture resource, including loading, unloading, and providing access to texture data.
 * Supports both SDL and OpenGL renderers.
 */
namespace Zephyrus::Assets {
	class Texture
	{
	private:
		std::string mFilePath;
		SDL_Texture* mSdlTexture = nullptr;
		int mWidth, mHeight;
		GLuint mTextureId = 0;
		// Loads a texture using OpenGL renderer
		bool LoadGl(Zephyrus::Render::RendererOpenGl* pRenderer, const std::string& pFilePath, SDL_Surface* pSurface);

	public:
		Texture();
		~Texture();

		// Loads a texture from file using the specified renderer
		bool Load(Zephyrus::Render::IRenderer& pRenderer, const std::string& pFilename);
		// Unloads the texture and frees associated resources
		void Unload();
		void SetActive() const;
		// Updates the width and height information of the texture
		void UpdateInfo(int& pWidth, int& pHeight);
		// Overrides the texture size with custom width and height
		void OverrideTextureSize(int pWidth, int pHeight);
		inline Vector3D GetTextureSize() const { return { static_cast<float>(mWidth), static_cast<float>(mHeight), 0 }; }
		SDL_Texture* GetSdlTexture() const { return mSdlTexture; }
		inline GLuint GetId() const { return mTextureId; }
		inline std::string GetTextureFilePath() const { return mFilePath; }
	};
}