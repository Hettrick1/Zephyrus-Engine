#include "FlipbookComponent.h"

#include "Timer.h"

FlipbookComponent::FlipbookComponent(Actor* pOwner, const std::vector<Texture*>& pTextures, int pDrawOrder)
	: SpriteComponent(pOwner, *pTextures[0], pDrawOrder), mCurrentFrame(0.0f), mAnimationFps(24.0f)
	, mHasFinished(true), mCanPlay(true), mCanPlayPending(false)
{
	SetAnimationTextures(pTextures);
}

FlipbookComponent::~FlipbookComponent()
{
	while (mAnimationTextures.size() > 0)
	{
		delete mAnimationTextures.back();
		mAnimationTextures.pop_back();
	}
	mAnimationTextures.clear();
}

void FlipbookComponent::SetAnimationTextures(const std::vector<Texture*>& pTextures)
{
	mAnimationTextures = pTextures;
	if (mAnimationTextures.size() > 0) 
	{
		SetTexture(*mAnimationTextures[0]);
		mCurrentFrame = 0.0f;
		mPlayOnce = false;
		mHasFinished = true;
	}
}

void FlipbookComponent::SetAnimationFps(float pFps)
{
	mAnimationFps = pFps;
}

void FlipbookComponent::PlayAnimation()
{
	mPlayOnce = true;
}

void FlipbookComponent::SetCanPlay(bool canPlay)
{
	mCanPlayPending = !canPlay;
}

void FlipbookComponent::SetIsLooping(bool looping)
{
	mIsLooping = looping;
}

void FlipbookComponent::Update()
{
	SpriteComponent::Update();
	if (mAnimationTextures.size() == 0) 
	{
		return;
	}
	if ((mIsLooping || mPlayOnce) && mCanPlay)
	{
		mHasFinished = false;
		mCurrentFrame += mAnimationFps * Timer::deltaTime;
		while (mCurrentFrame >= mAnimationTextures.size())
		{
			mCurrentFrame -= mAnimationTextures.size();
			mPlayOnce = false;
			mHasFinished = true;
			if (mCanPlayPending) {
				mCanPlay = false;
			}
		}
		if (mCanPlay)
		{
			SetTexture(*mAnimationTextures[static_cast<int>(mCurrentFrame)]);
		}
	}
	else if (!mCanPlay)
	{
		mCurrentFrame = mAnimationTextures.size() - 1;
		SetTexture(*mAnimationTextures[static_cast<int>(mCurrentFrame)]);
	}
}
