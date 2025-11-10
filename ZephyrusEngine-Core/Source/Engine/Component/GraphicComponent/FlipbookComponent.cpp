#include "FlipbookComponent.h"

#include "Timer.h"
#include "Assets.h"
#include "Interface/ITexture2D.h"

using Zephyrus::Assets::AssetsManager;

namespace Zephyrus::ActorComponent
{
	FlipbookComponent::FlipbookComponent(Actor* pOwner, int pDrawOrder)
		: SpriteComponent(pOwner, "FlipbookComponent"), mCurrentFrame(0.0f), mAnimationFps(24.0f)
		, mHasFinished(true), mCanPlay(true), mCanPlayPending(false), mPlayOnce(false), mIsLooping(false), mAnimationTextures({})
	{
		if (mAnimationTextures.size() > 0)
		{
			SetTexture(mAnimationTextures[0]);
			mCurrentFrame = 0.0f;
			mPlayOnce = false;
			mHasFinished = true;
		}
	}

	FlipbookComponent::~FlipbookComponent()
	{
		mAnimationTextures.clear();
	}

	void FlipbookComponent::OnStart()
	{
		Component::OnStart();
		if (mAnimationTextures.empty())
		{
			Assets::ITexture2D* fallbackTexture = AssetsManager::LoadTexture("../Content/Sprites/square.png", "../Content/Sprites/square.png");
			SetTexture(fallbackTexture);
			mAnimationTextures.push_back(fallbackTexture);
		}
	}

	void FlipbookComponent::OnEnd()
	{
		SpriteComponent::OnEnd();
	}

	std::vector<PropertyDescriptor> FlipbookComponent::GetProperties()
	{
		if (!mAnimationTextures.empty())
		{
			SetTexture(mAnimationTextures[0]);
		}
		return {
			{ "Textures", &mAnimationTextures, PropertyType::VectorTexture },
			{ "Is Looping : ", &mIsLooping, PropertyType::Bool},
			{ "Animation FPS : ", &mAnimationFps, PropertyType::Float},
			{ "Material Instance : ",&mMaterial, PropertyType::MaterialInstance },
		};
	}

	void FlipbookComponent::Deserialize(Serialization::IDeserializer& pReader)
	{
		SpriteComponent::Deserialize(pReader);

		if (auto fbTextures = pReader.ReadArrayString("textures"))
		{
			const auto& arr = *fbTextures;

			if (!arr.empty())
			{
				for (auto& element : arr)
				{
					Assets::ITexture2D* texture = AssetsManager::LoadTexture(element, element);
					AddAnimationTexture(texture);
				}
				if (mAnimationTextures.size() > 0)
				{
					SetTexture(mAnimationTextures[0]);
					mCurrentFrame = 0.0f;
					mPlayOnce = false;
					mHasFinished = true;
				}
			}
		}
		if (auto animFps = pReader.ReadFloat("animFps"))
		{
			SetAnimationFps(*animFps);
		}
		if (auto looping = pReader.ReadBool("isLooping"))
		{
			mIsLooping = *looping;
		}
	}

	void FlipbookComponent::Serialize(Serialization::ISerializer& pWriter)
	{
		Component::BeginSerialize(pWriter);
		pWriter.WriteBool("cullOff", mCullOff);
		pWriter.BeginArray("textures");
		for (auto& texture : mAnimationTextures)
		{
			pWriter.PushString(texture->GetFilePath());
		}
		pWriter.EndArray();
		pWriter.WriteFloat("animFps", mAnimationFps);
		pWriter.WriteBool("isLooping", mIsLooping);
		RenderComponent::Serialize(pWriter);
		Component::EndSerialize(pWriter);
	}

	void FlipbookComponent::SetAnimationTextures(const std::vector<Assets::ITexture2D*>& pTextures)
	{
		mAnimationTextures = pTextures;
		if (mAnimationTextures.size() > 0)
		{
			SetTexture(mAnimationTextures[0]);
			mCurrentFrame = 0.0f;
			mPlayOnce = false;
			mHasFinished = true;
		}
	}

	void FlipbookComponent::AddAnimationTexture(Assets::ITexture2D* pTexture)
	{
		if (std::find(mAnimationTextures.begin(), mAnimationTextures.end(), pTexture) == mAnimationTextures.end())
		{
			mAnimationTextures.push_back(pTexture);
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

	void FlipbookComponent::SetCanPlay(bool pCanPlay)
	{
		mCanPlayPending = !pCanPlay;
	}

	void FlipbookComponent::SetIsLooping(bool pLooping)
	{
		mIsLooping = pLooping;
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
				SetTexture(mAnimationTextures[static_cast<int>(mCurrentFrame)]);
			}
		}
		else if (!mCanPlay)
		{
			mCurrentFrame = static_cast<float>(mAnimationTextures.size() - 1);
			SetTexture(mAnimationTextures[static_cast<int>(mCurrentFrame)]);
		}
	}
}