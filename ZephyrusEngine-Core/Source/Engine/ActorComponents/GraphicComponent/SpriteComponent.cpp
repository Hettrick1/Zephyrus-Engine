#include "SpriteComponent.h"

#include "Actor.h"
#include "../../Scenes/Scene.h"
#include "FactoryJSON/JSONUtils.h"

SpriteComponent::SpriteComponent(Actor* pOwner)
	: Component(pOwner), mTexture(), mDrawOrder(100), mFlipMethode(IRenderer::Flip::None)
{
}

SpriteComponent::~SpriteComponent()
{
}

void SpriteComponent::Deserialize(const rapidjson::Value& pData)
{
	Component::Deserialize(pData);
	if (pData.HasMember("texture") && pData["texture"].IsString())
	{
		mTexture = *Assets::LoadTexture(pData["texture"].GetString(), pData["texture"].GetString());
	}
	mTexWidth = static_cast<int>(mTexture.GetTextureSize().x);
	mTexHeight = static_cast<int>(mTexture.GetTextureSize().y);
	if (pData.HasMember("sizeOverride") && pData["sizeOverride"].IsArray())
	{
		if (auto sizeOverride = ReadVector3D(pData, "sizeOverride"))
		{
			Vector3D size = *sizeOverride;
			if (size.x > 0 || size.y > 0) {
				mTexWidth = static_cast<int>(mTexture.GetTextureSize().x);
				mTexHeight = static_cast<int>(mTexture.GetTextureSize().y);
			}
		}
	}
	aspectRatio = static_cast<float>(mTexWidth) / static_cast<float>(mTexHeight);
	aspectRatioInv = 1 / aspectRatio;
	mOwner->GetScene().GetRenderer()->AddSprite(this);
}

void SpriteComponent::SetTexture(const Texture& pTexture)
{
	mTexture = pTexture;
	if (mTexHeightOverride == 0 || mTexWidthOverride == 0)
	{
		mTexture.UpdateInfo(mTexWidth, mTexHeight);
	}
	else 
	{
		mTexWidth = mTexWidthOverride;
		mTexHeight = mTexHeightOverride;
		mTexture.OverrideTextureSize(mTexWidth, mTexHeight);
	}
}

void SpriteComponent::SetFlipMethode(IRenderer::Flip pFlipMethode)
{
	mFlipMethode = pFlipMethode;
}

void SpriteComponent::Draw(const IRenderer& pRenderer)
{
	if (mOwner->GetState() == ActorState::Active)
	{
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);
		glDisable(GL_CULL_FACE);
		if (mCullOff)
		{
			glDisable(GL_DEPTH_TEST);
		}
		pRenderer.DrawSprite(*mOwner, mTexture, Rectangle(), Vector2D(), mFlipMethode);
	}
}

void SpriteComponent::SetCullOff(bool cull)
{
	mCullOff = cull;
}

Matrix4DRow SpriteComponent::GetWorldTransform()
{
	if (mOwner)
	{
		Vector3D spriteSize = mRelativeSize;

		if (mTexWidth > mTexHeight)
		{
			spriteSize.x *= aspectRatio;
		}
		else
		{
			spriteSize.y *= aspectRatioInv;
		}

		mRelativeTransform = Matrix4DRow::CreateScale(spriteSize);
		mRelativeTransform *= Matrix4DRow::CreateFromQuaternion(mRelativeRotation);
		mRelativeTransform *= Matrix4DRow::CreateTranslation(mRelativePosition);

		return mRelativeTransform * mOwner->GetTransformComponent().GetWorldTransform();
	}

	return mRelativeTransform;
}
