#pragma once
#include "Component.h"
#include "../Graphics/Texture.h"

/**
 * @brief Component that handles rendering a 2D sprite for an Actor.
 * Manages the texture, draw order, flipping, culling, and size override for the sprite.
 */
class SpriteComponent : public Component
{
public:
	SpriteComponent(Actor* pOwner, Texture& pTexture, int pDrawOrder = 100, Vector3D pSizeOverride = 0);
	virtual ~SpriteComponent();
	SpriteComponent() = delete;
	SpriteComponent(const SpriteComponent&) = delete;
	SpriteComponent& operator= (const SpriteComponent&) = delete;

	// Sets the texture used by the sprite
	virtual void SetTexture(const Texture& pTexture);
	// Sets the flip method for rendering the sprite
	void SetFlipMethode(IRenderer::Flip pFlipMethode);
	virtual void Draw(IRenderer& pRenderer);

	int GetDrawOrder() const { return mDrawOrder; }
	int GetTexWidth() const { return mTexWidth; }
	int GetTexHeight() const { return mTexHeight; }

	// Enables or disables culling for the sprite
	void SetCullOff(bool cull);
	// Returns the world transform matrix for the sprite
	Matrix4DRow GetWorldTransform() override;

	Texture& GetTexture();
protected:
	Texture mTexture;
	int mDrawOrder = 0;
	int mTexWidth = 0;
	int mTexHeight = 0;
	int mTexWidthOverride = 0;
	int mTexHeightOverride = 0;
	IRenderer::Flip mFlipMethode = IRenderer::Flip::None;
	bool mCullOff = false;
	float aspectRatio = 0.0, aspectRatioInv = 0.0;
};
