#pragma once

#include "SpriteComponent.h"

/**
 * @brief Component that handles animated flipbook sprites for an Actor.
 * It manages a sequence of textures and plays them as an animation,
 * supporting looping, play control, and frame rate adjustment.
 */
class FlipbookComponent : public SpriteComponent
{
public:
	FlipbookComponent(Actor* pOwner, const std::vector<Texture*>& pTextures, int pDrawOrder = 100);
	virtual ~FlipbookComponent();
	FlipbookComponent() = delete;
	FlipbookComponent(const FlipbookComponent&) = delete;
	FlipbookComponent& operator=(const FlipbookComponent&) = delete;
public:
	float GetAnimationFps() const { return mAnimationFps; }
	// Sets the textures used for the animation
	void SetAnimationTextures(const std::vector<Texture*>& pTextures);
	// Sets the animation frame rate (frames per second)
	void SetAnimationFps(float pFps);
	// Starts playing the animation from the beginning
	void PlayAnimation();
	inline bool IsAnimationEnded() const { return mHasFinished; }

	// Enables or disables animation playback
	void SetCanPlay(bool pCanPlay);
	// Sets whether the animation should loop
	void SetIsLooping(bool pLooping);

	void Update() override;
private:
	std::vector<Texture*> mAnimationTextures;
	float mCurrentFrame;
	float mAnimationFps;
	bool mIsLooping, mPlayOnce, mHasFinished, mCanPlay, mCanPlayPending;
};
