#include "FlipbookComponent.h"

#include "Timer.h"
#include "Assets.h"

namespace Zephyrus::ActorComponent
{
	FlipbookComponent::FlipbookComponent(Actor* pOwner, int pDrawOrder)
		: SpriteComponent(pOwner, "FlipbookComponent"), mCurrentFrame(0.0f), mAnimationFps(24.0f)
		, mHasFinished(true), mCanPlay(true), mCanPlayPending(false), mPlayOnce(false), mIsLooping(false), mAnimationTextures({})
	{
		if (mAnimationTextures.size() > 0)
		{
			SetTexture(*mAnimationTextures[0]);
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
			Texture* fallbackTexture = AssetsManager::LoadTexture("../Content/Sprites/square.png", "../Content/Sprites/square.png");
			SetTexture(*fallbackTexture);
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
			SetTexture(*mAnimationTextures[0]);
		}
		return {
			{ "Textures", &mAnimationTextures, PropertyType::VectorTexture },
			{ "Is Looping : ", &mIsLooping, PropertyType::Bool},
			{ "Animation FPS : ", &mAnimationFps, PropertyType::Float}
		};
	}

	void FlipbookComponent::Deserialize(const rapidjson::Value& pData)
	{
		SpriteComponent::Deserialize(pData);

		if (pData.HasMember("textures") && pData["textures"].IsArray())
		{
			const auto& arr = pData["textures"].GetArray();

			if (!arr.Empty())
			{
				for (auto& element : arr)
				{
					if (element.IsString())
					{
						Texture* texture = AssetsManager::LoadTexture(element.GetString(), element.GetString());
						AddAnimationTexture(texture);
					}
				}
				if (mAnimationTextures.size() > 0)
				{
					SetTexture(*mAnimationTextures[0]);
					mCurrentFrame = 0.0f;
					mPlayOnce = false;
					mHasFinished = true;
				}
			}
		}
		if (pData.HasMember("animFps") && pData["animFps"].IsFloat())
		{
			SetAnimationFps(pData["animFps"].GetFloat());
		}
		if (auto looping = Serialization::Json::ReadBool(pData, "isLooping"))
		{
			mIsLooping = *looping;
		}
	}

	void FlipbookComponent::Serialize(Serialization::Json::JsonWriter& pWriter)
	{
		Component::BeginSerialize(pWriter);
		pWriter.WriteBool("cullOff", mCullOff);
		pWriter.BeginArray("textures");
		for (auto& texture : mAnimationTextures)
		{
			pWriter.PushString(texture->GetTextureFilePath());
		}
		pWriter.EndArray();
		pWriter.WriteFloat("animFps", mAnimationFps);
		pWriter.WriteBool("isLooping", mIsLooping);
		Component::EndSerialize(pWriter);
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

	void FlipbookComponent::AddAnimationTexture(Texture* pTexture)
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
				SetTexture(*mAnimationTextures[static_cast<int>(mCurrentFrame)]);
			}
		}
		else if (!mCanPlay)
		{
			mCurrentFrame = static_cast<float>(mAnimationTextures.size() - 1);
			SetTexture(*mAnimationTextures[static_cast<int>(mCurrentFrame)]);
		}
	}
}