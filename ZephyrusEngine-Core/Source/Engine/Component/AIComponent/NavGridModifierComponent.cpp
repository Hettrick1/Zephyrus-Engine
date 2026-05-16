#include "pch.h"

#include "NavGridModifierComponent.h"
#include "NavGridVolumeComponent.h"
#include "AI/NavGridManager.h"
#include "DebugRenderer.h"
#include "OrientedBoundingBox.h"

namespace Zephyrus::ActorComponent
{
	struct NavGridModifierComponent::Impl
	{
		NavGridModifierComponent::Impl() = default;
		Vector3D mVolumeSize{ 10.0f, 10.0f, 10.0f };
		Debug::PersistantDebugBox mDebugBoxBackup = Debug::PersistantDebugBox(Matrix4DRow::Identity, Vector3D(0.0, 0.0, 1.0));
		Physics::OrientedBoundingBox mObb = Physics::OrientedBoundingBox(0, 0, Matrix4DRow::Identity);

		float mWeight = 1.0f;

	};
	
	NavGridModifierComponent::NavGridModifierComponent(Actor* pOwner)
		: Component(pOwner, "NavGridModifierComponent"), mImpl(std::make_unique<Impl>())
	{
		mOwner->GetSceneContext()->GetNavGridManager()->AddModifierComponent(this);
	}

	NavGridModifierComponent::~NavGridModifierComponent()
	{
		mOwner->GetSceneContext()->GetNavGridManager()->RemoveModifierComponent(this);
		mOwner->GetSceneContext()->GetRenderer()->GetDebugRenderer()->RemovePersistantDebugBox(mImpl->mDebugBoxBackup);
	}

	void NavGridModifierComponent::Deserialize(Serialization::IDeserializer& pReader)
	{
		Component::Deserialize(pReader);
		if (auto volumeSize = pReader.ReadVector3D("volumeSize"))
		{
			mImpl->mVolumeSize = *volumeSize;
		}

		if (auto weight = pReader.ReadFloat("weight"))
		{
			mImpl->mWeight = *weight;
		}

		auto rotationMatrix = Matrix4DRow::CreateFromQuaternion(mOwner->GetTransformComponent().GetRotation());
		mImpl->mObb = Physics::OrientedBoundingBox(GetWorldPosition(), mImpl->mVolumeSize, rotationMatrix);

		Matrix4DRow wt;
		wt = Matrix4DRow::CreateScale(mImpl->mVolumeSize * 2.0f);
		wt *= rotationMatrix;
		wt *= Matrix4DRow::CreateTranslation(GetWorldPosition());

		Debug::PersistantDebugBox box = Debug::PersistantDebugBox(wt, Vector3D(0.0, 0.0, 1.0));

		mOwner->GetSceneContext()->GetRenderer()->GetDebugRenderer()->AddPersistantDebugBox(box);
		mImpl->mDebugBoxBackup = box;
	}

	void NavGridModifierComponent::Serialize(Serialization::ISerializer& pWriter)
	{
		Component::BeginSerialize(pWriter);
		pWriter.WriteVector3D("volumeSize", mImpl->mVolumeSize);
		pWriter.WriteFloat("weight", mImpl->mWeight);
		Component::EndSerialize(pWriter);
	}

	std::vector<PropertyDescriptor> NavGridModifierComponent::GetProperties()
	{
		Matrix4DRow wt;

		auto rotationMatrix = Matrix4DRow::CreateFromQuaternion(mOwner->GetTransformComponent().GetRotation());
		wt = Matrix4DRow::CreateScale(mImpl->mVolumeSize * 2.0f);
		wt *= rotationMatrix;
		wt *= Matrix4DRow::CreateTranslation(GetWorldPosition());

		Debug::PersistantDebugBox box = Debug::PersistantDebugBox(wt, Vector3D(0.0, 0.0, 1.0));

		if (box != mImpl->mDebugBoxBackup)
		{
			mOwner->GetSceneContext()->GetRenderer()->GetDebugRenderer()->RemovePersistantDebugBox(mImpl->mDebugBoxBackup);
			mOwner->GetSceneContext()->GetRenderer()->GetDebugRenderer()->AddPersistantDebugBox(box);
			mImpl->mDebugBoxBackup = box;
			mImpl->mObb = Physics::OrientedBoundingBox(GetWorldPosition(), mImpl->mVolumeSize, rotationMatrix);
		}

		return
		{
			{ "Volume Size : ", &mImpl->mVolumeSize, PropertyType::Vec3},
			{ "Weight : ", &mImpl->mWeight, PropertyType::Float}
		};
	}

	void NavGridModifierComponent::OnStart()
	{
		Component::OnStart();
	}

	bool NavGridModifierComponent::IsPointInsideVolume(const Vector3D& point) const
	{
		return mImpl->mObb.IsPointInOBB(point);
	}
	float NavGridModifierComponent::GetWeight() const
	{
		return mImpl->mWeight;
	}
}
