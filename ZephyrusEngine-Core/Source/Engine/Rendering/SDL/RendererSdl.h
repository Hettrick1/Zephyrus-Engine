#pragma once

#include "IRenderer.h"
#include <vector>

class Actor;
class Texture;

/**
 * @brief SDL-based renderer implementation for 2D sprite and sky sphere rendering.
 * Manages the SDL renderer, sprite components, and sky sphere component.
 * 
 * INFO : Needs to be patch
 */
class RendererSdl : public IRenderer
{
private:
	SDL_Renderer* mSdlRenderer = nullptr;
	std::vector<SpriteComponent*> mSprites;
public:
	RendererSdl();
	RendererSdl(const RendererSdl&) = delete;
	RendererSdl& operator = (const RendererSdl&) = delete;

	// Initialize the SDL renderer with the given window
	bool Initialize(Window& pWindow);
	void BeginDraw() override;
	void Draw() override;
	void EndDraw() override;
	void Close() override;
	void Unload() override;

	void AddSprite(SpriteComponent* pSprite) override;
	void RemoveSprite(SpriteComponent* pSprite) override;

	void AddSkySphere(SkySphereComponent* pSkySphere) override;
	void RemoveSkySphere() override;

	RendererType GetType() const override { return IRenderer::RendererType::SDL; }

	// Draw a rectangle using SDL
	void DrawRect(Rectangle& pRect);
	// Draw a sprite for the given actor and texture
	void DrawSprite(Actor& pActor, Texture& pTexture, Rectangle pRect, Vector2D pOrigin, IRenderer::Flip pFlipMethod) const override;
	// Returns the underlying SDL_Renderer pointer
	SDL_Renderer* ToSdlRenderer() override;
};
