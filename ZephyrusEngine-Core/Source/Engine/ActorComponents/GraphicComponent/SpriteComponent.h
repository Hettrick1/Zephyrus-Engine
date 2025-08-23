#pragma once
#include "Component.h"
#include "../Graphics/Texture.h"

/**
 * @brief Component that handles rendering a 2D sprite for an Actor.
 * Manages the texture, draw order, flipping, culling, and size override for the sprite.
 */
class SpriteComponent : public Component
{
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
public:
	SpriteComponent(Actor* pOwner);
	virtual ~SpriteComponent();

	void Deserialize(const rapidjson::Value& pData) override;

	static Component* Create(Actor* pOwner) { return new SpriteComponent(pOwner); }

	// Sets the texture used by the sprite
	virtual void SetTexture(const Texture& pTexture);
	// Sets the flip method for rendering the sprite
	void SetFlipMethode(IRenderer::Flip pFlipMethode);
	virtual void Draw(const IRenderer& pRenderer);

	// Enables or disables culling for the sprite
	void SetCullOff(bool cull);
	// Returns the world transform matrix for the sprite
	Matrix4DRow GetWorldTransform() override;

	inline Texture& GetTexture() { return mTexture; }
	inline int GetDrawOrder() const { return mDrawOrder; }
	inline int GetTexWidth() const { return mTexWidth; }
	inline int GetTexHeight() const { return mTexHeight; }
};
