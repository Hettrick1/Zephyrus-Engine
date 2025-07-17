#pragma once

#include <iostream>
#include "SDL.h"
#include "SDL_image.h"
#include "Vector2D.h"
#include "IRenderer.h"

class RendererSdl;
class RendererOpenGl;

/**
 * @brief Manages a texture resource, including loading, unloading, and providing access to texture data.
 * Supports both SDL and OpenGL renderers.
 */
class Texture
{
public:
	Texture();
	~Texture();

	// Loads a texture from file using the specified renderer
	bool Load(IRenderer& pRenderer, const std::string& pFilename);
	// Unloads the texture and frees associated resources
	void Unload();
	void SetActive() const;
	// Updates the width and height information of the texture
	void UpdateInfo(int& pWidth, int& pHeight);
	// Overrides the texture size with custom width and height
	void OverrideTextureSize(int pWidth, int pHeight);
	Vector3D GetTextureSize();
	SDL_Texture* GetSdlTexture() const { return mSdlTexture; }
	inline unsigned int& GetId() { return mTextureId; }
 
private:
	std::string mFilePath;
	SDL_Texture* mSdlTexture;
	int mWidth, mHeight;
	unsigned int mTextureId;
	// Loads a texture using SDL renderer
	bool LoadSdl(RendererSdl* pRenderer, const std::string& pFilePath, SDL_Surface* pSurface);
	// Loads a texture using OpenGL renderer
	bool LoadGl(RendererOpenGl* pRenderer, const std::string& pFilePath, SDL_Surface* pSurface);
};