#include "SpriteComponent.h"

#include "Actor.h"
#include "../../Scenes/Scene.h"

SpriteComponent::SpriteComponent(Actor* pOwner, Texture& pTexture, int pDrawOrder, Vector3D pSizeOverride)
	: Component(pOwner), mTexture(pTexture), mDrawOrder(pDrawOrder), mFlipMethode(IRenderer::Flip::None), mCullOff(false)
	, aspectRatio(0), aspectRatioInv(0), mTexWidthOverride(0), mTexHeightOverride(0)
{
	if (pSizeOverride.x == 0 || pSizeOverride.y == 0) {
		mTexWidth = static_cast<int>(pTexture.GetTextureSize().x);
		mTexHeight = static_cast<int>(pTexture.GetTextureSize().y);
	}
	else {
		mTexWidthOverride = static_cast<int>(pSizeOverride.x);
		mTexHeightOverride = static_cast<int>(pSizeOverride.y);
		mTexWidth = mTexWidthOverride;
		mTexHeight = mTexHeightOverride;
	}
	aspectRatio = static_cast<float>(mTexWidth) / static_cast<float>(mTexHeight);
	aspectRatioInv = 1 / aspectRatio;
	mOwner->GetScene().GetRenderer()->AddSprite(this);
}

SpriteComponent::~SpriteComponent()
{
	mOwner->GetScene().GetRenderer()->RemoveSprite(this);
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

void SpriteComponent::Draw(IRenderer& pRenderer)
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

Texture& SpriteComponent::GetTexture()
{
	return mTexture;
}
