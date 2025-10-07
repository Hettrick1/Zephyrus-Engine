#pragma once
#include "Component.h"
#include "Texture.h"

/**
 * @brief Component that handles rendering a 2D sprite for an Actor.
 * Manages the texture, draw order, flipping, culling, and size override for the sprite.
 */
namespace Zephyrus::ActorComponent
{
	class SpriteComponent : public Component
	{
	protected:
		Assets::ITexture* mTexture;
		int mDrawOrder = 0;
		int mTexWidth = 0;
		int mTexHeight = 0;
		int mTexWidthOverride = 0;
		int mTexHeightOverride = 0;
		Zephyrus::Render::IRenderer::Flip mFlipMethode = Zephyrus::Render::IRenderer::Flip::None;
		bool mCullOff = false;
		float aspectRatio = 0.0, aspectRatioInv = 0.0;
	public:
		SpriteComponent(Actor* pOwner, const std::string& pName);
		virtual ~SpriteComponent();

		virtual void Update() override;
		virtual void OnEnd() override;

		virtual std::vector<PropertyDescriptor> GetProperties() override;

		void Deserialize(Serialization::IDeserializer& pReader) override;
		void Serialize(Serialization::ISerializer& pWriter) override;

		static Component* Create(Actor* pOwner) { return new SpriteComponent(pOwner, "SpriteComponent"); }

		// Sets the texture used by the sprite
		virtual void SetTexture(Assets::ITexture* pTexture);
		// Sets the flip method for rendering the sprite
		void SetFlipMethode(Zephyrus::Render::IRenderer::Flip pFlipMethode);
		virtual void Draw(const Zephyrus::Render::IRenderer& pRenderer);

		// Enables or disables culling for the sprite
		void SetCullOff(bool cull);
		// Returns the world transform matrix for the sprite
		Matrix4DRow GetWorldTransform() override;

		void SetDrawOrder(const int pOrder);

		inline Assets::ITexture* GetTexture() { return mTexture; }
		inline int GetDrawOrder() const { return mDrawOrder; }
		inline int GetTexWidth() const { return mTexWidth; }
		inline int GetTexHeight() const { return mTexHeight; }
	};
}
