#pragma once

#include "SpriteComponent.h"

/**
 * @brief Component that handles animated flipbook sprites for an Actor.
 * It manages a sequence of textures and plays them as an animation,
 * supporting looping, play control, and frame rate adjustment.
 */
class FlipbookComponent : public SpriteComponent
{
private:
	std::vector<Texture*> mAnimationTextures;
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

	void Deserialize(const rapidjson::Value& pData) override;
	void Serialize(Serialization::Json::JsonWriter& pWriter) override;

	static Component* Create(Actor* pOwner) { return new FlipbookComponent(pOwner); }

	// Sets the textures used for the animation
	void SetAnimationTextures(const std::vector<Texture*>& pTextures);
	void AddAnimationTexture(Texture* pTexture);
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
