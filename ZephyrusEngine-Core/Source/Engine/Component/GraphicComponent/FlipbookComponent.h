#pragma once

#include "SpriteComponent.h"

/**
 * @brief Component that handles animated flipbook sprites for an Actor.
 * It manages a sequence of textures and plays them as an animation,
 * supporting looping, play control, and frame rate adjustment.
 */

namespace Zephyrus::ActorComponent
{
	class FlipbookComponent : public SpriteComponent
	{
	private:
		std::vector<Assets::ITexture2D* > mAnimationTextures;
		float mCurrentFrame;
		float mAnimationFps;
		bool mIsLooping, mPlayOnce, mHasFinished, mCanPlay, mCanPlayPending;
	public:
		FlipbookComponent(Actor* pOwner, int pDrawOrder = 100);
		virtual ~FlipbookComponent();
		FlipbookComponent() = delete;
		FlipbookComponent(const FlipbookComponent&) = delete;
		FlipbookComponent& operator=(const FlipbookComponent&) = delete;

		void OnStart() override;
		void OnEnd() override;
		std::vector<PropertyDescriptor> GetProperties() override;

		void Deserialize(Serialization::IDeserializer& pReader) override;
		void Serialize(Serialization::ISerializer& pWriter) override;

		static Component* Create(Actor* pOwner) { return new FlipbookComponent(pOwner); }

		// Sets the textures used for the animation
		void SetAnimationTextures(const std::vector<Assets::ITexture2D*>& pTextures);
		void AddAnimationTexture(Assets::ITexture2D* pTexture);
		// Sets the animation frame rate (frames per second)
		void SetAnimationFps(float pFps);
		// Starts playing the animation from the beginning
		void PlayAnimation();

		// Enables or disables animation playback
		void SetCanPlay(bool pCanPlay);
		void SetIsLooping(bool pLooping);

		void Update() override;

		inline bool IsAnimationEnded() const { return mHasFinished; }
		inline float GetAnimationFps() const { return mAnimationFps; }
	};
}