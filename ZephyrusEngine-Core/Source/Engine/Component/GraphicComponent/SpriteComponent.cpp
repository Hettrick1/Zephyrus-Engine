#include "SpriteComponent.h"

#include "Actor.h"
#include "Scene.h"
#include "JSONUtils.h"
#include "DebugRenderer.h"
#include "Interface/ITexture2D.h"

using Zephyrus::Assets::AssetsManager;

namespace Zephyrus::ActorComponent
{
	SpriteComponent::SpriteComponent(Actor* pOwner, const std::string& pName)
		: RenderComponent(pOwner, pName), mTexture(), mDrawOrder(100), mFlipMethode(Zephyrus::Render::IRenderer::Flip::None)
	{
		mOwner->GetScene().GetRenderer()->AddSprite(this);
		mTexture = AssetsManager::LoadTexture("../Content/Sprites/uv_mapper.jpg", "../Content/Sprites/uv_mapper.jpg");
		mTexWidth = static_cast<int>(mTexture->GetWidth());
		mTexHeight = static_cast<int>(mTexture->GetHeight());
		aspectRatio = static_cast<float>(mTexWidth) / static_cast<float>(mTexHeight);
		aspectRatioInv = 1 / aspectRatio;

		/*auto mat = Assets::AssetsManager::LoadMaterial("../Content/Material/BasicSprite.zpmat", "../Content/Material/BasicSprite.zpmat");
		SetMaterial(mat);*/
	}

	SpriteComponent::~SpriteComponent()
	{

	}

	void SpriteComponent::Update()
	{
		Component::Update();
	}

	void SpriteComponent::OnEnd()
	{
		mOwner->GetScene().GetRenderer()->RemoveSprite(this);
		Component::OnEnd();
	}

	std::vector<PropertyDescriptor> SpriteComponent::GetProperties()
	{
		return {
			 { "Texture", &mTexture, PropertyType::Texture2D },
			 { "Material Instance : ",& mMaterial, PropertyType::MaterialInstance },
		};
	}

	void SpriteComponent::Deserialize(Serialization::IDeserializer& pReader)
	{
		RenderComponent::Deserialize(pReader);
		if (auto texturePath = pReader.ReadString("texture"))
		{
			SetTexture(AssetsManager::LoadTexture(*texturePath, *texturePath));
		}
		mTexWidth = static_cast<int>(mTexture->GetWidth());
		mTexHeight = static_cast<int>(mTexture->GetHeight());
		if (auto texturePath = pReader.ReadBool("cullOff"))
		{
			SetCullOff(*texturePath);
		}
		aspectRatio = static_cast<float>(mTexWidth) / static_cast<float>(mTexHeight);
		aspectRatioInv = 1 / aspectRatio;
	}

	void SpriteComponent::Serialize(Serialization::ISerializer& pWriter)
	{
		Component::BeginSerialize(pWriter);
		pWriter.WriteString("texture", mTexture->GetFilePath());
		pWriter.WriteBool("cullOff", mCullOff);
		RenderComponent::Serialize(pWriter);
		Component::EndSerialize(pWriter);
	}

	void SpriteComponent::SetTexture(Assets::ITexture2D* pTexture)
	{
		mTexture = pTexture;
		//if (mTexHeightOverride == 0 || mTexWidthOverride == 0)
		//{
		//	mTexture->UpdateInfo(mTexWidth, mTexHeight);
		//}
		//else
		//{
		//	mTexWidth = mTexWidthOverride;
		//	mTexHeight = mTexHeightOverride;
		//	mTexture->OverrideTextureSize(mTexWidth, mTexHeight);
		//}
		mMaterial.SetTexture("albedo", mTexture);
		aspectRatio = static_cast<float>(mTexWidth) / static_cast<float>(mTexHeight);
		aspectRatioInv = 1 / aspectRatio;
	}

	void SpriteComponent::SetFlipMethode(Zephyrus::Render::IRenderer::Flip pFlipMethode)
	{
		mFlipMethode = pFlipMethode;
	}

	void SpriteComponent::Draw(const Zephyrus::Render::IRenderer& pRenderer)
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
			pRenderer.DrawSprite(mMaterial, GetWorldTransform());
		}
	}

	void SpriteComponent::SetCullOff(bool cull)
	{
		mCullOff = cull;
	}

	Matrix4DRow SpriteComponent::GetWorldTransform()
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

		if (mParent)
		{
			return mRelativeTransform * mParent->GetWorldTransform();
		}
		else if (mOwner)
		{
			return  mRelativeTransform * mOwner->GetTransformComponent().GetWorldTransform();
		}
		return mRelativeTransform;
	}

	void SpriteComponent::SetDrawOrder(const int pOrder)
	{
		mDrawOrder = pOrder;
	}
}
