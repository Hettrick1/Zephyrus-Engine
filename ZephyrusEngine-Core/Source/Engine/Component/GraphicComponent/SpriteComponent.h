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
		Zephyrus::Assets::Texture mTexture;
		int mDrawOrder = 0;
		int mTexWidth = 0;
		int mTexHeight = 0;
		int mTexWidthOverride = 0;
		int mTexHeightOverride = 0;
		IRenderer::Flip mFlipMethode = IRenderer::Flip::None;
		bool mCullOff = false;
		float aspectRatio = 0.0, aspectRatioInv = 0.0;
	public:
		SpriteComponent(Actor* pOwner, const std::string& pName);
		virtual ~SpriteComponent();

		virtual void Update() override;
		virtual void OnEnd() override;

		virtual std::vector<PropertyDescriptor> GetProperties() override;

		void Deserialize(const rapidjson::Value& pData) override;
		void Serialize(Serialization::Json::JsonWriter& pWriter) override;

		static Component* Create(Actor* pOwner) { return new SpriteComponent(pOwner, "SpriteComponent"); }

		// Sets the texture used by the sprite
		virtual void SetTexture(const Zephyrus::Assets::Texture& pTexture);
		// Sets the flip method for rendering the sprite
		void SetFlipMethode(IRenderer::Flip pFlipMethode);
		virtual void Draw(const IRenderer& pRenderer);

		// Enables or disables culling for the sprite
		void SetCullOff(bool cull);
		// Returns the world transform matrix for the sprite
		Matrix4DRow GetWorldTransform() override;

		void SetDrawOrder(const int pOrder);

		inline Zephyrus::Assets::Texture& GetTexture() { return mTexture; }
		inline int GetDrawOrder() const { return mDrawOrder; }
		inline int GetTexWidth() const { return mTexWidth; }
		inline int GetTexHeight() const { return mTexHeight; }
	};
}
