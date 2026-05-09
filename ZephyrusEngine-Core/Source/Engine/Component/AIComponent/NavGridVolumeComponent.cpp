#include "pch.h"
#include "NavGridVolumeComponent.h"
#include "AI/NavGridManager.h"
#include "DebugRenderer.h"

namespace Zephyrus::ActorComponent
{
	struct NavGridVolumeComponent::Impl
	{
		NavGridVolumeComponent::Impl() = default;

		Vector3D mGridSize{ 20.0f, 20.0f, 50.0f };
		Vector2D mNumberOfPoint = { 20.0f, 20.0f };
		float mAgentWidth = 0.8f;
		float mAgentHeight = 1.5f;
		bool mShowLines = false;
		bool mShowNodePos = true;
		bool mShowAgentCollision = false;
		Matrix4DRow mWorldTransformBackup;
	};

	NavGridVolumeComponent::NavGridVolumeComponent(Actor* pOwner)
		: Component(pOwner, "NavGridVolumeComponent"), mImpl(std::make_unique<Impl>())
	{
		mOwner->GetSceneContext()->GetNavGridManager()->AddVolumeComponent(this);
	}

	NavGridVolumeComponent::~NavGridVolumeComponent()
	{
		mOwner->GetSceneContext()->GetNavGridManager()->RemoveVolumeComponent(this);
		mOwner->GetSceneContext()->GetRenderer()->GetDebugRenderer()->RemovePersistantDebugBox(mImpl->mWorldTransformBackup);
	}

	void NavGridVolumeComponent::Deserialize(Serialization::IDeserializer& pReader)
	{
		Component::Deserialize(pReader);
		if (auto gridSize = pReader.ReadVector3D("gridSize"))
		{
			mImpl->mGridSize = *gridSize;
		}
		if (auto nbOfPoints = pReader.ReadVector2D("nbOfPoints"))
		{
			mImpl->mNumberOfPoint = *nbOfPoints;
		}
		if (auto agentWidth = pReader.ReadFloat("agentWidth"))
		{
			mImpl->mAgentWidth = *agentWidth;
		}
		if (auto agentHeight = pReader.ReadFloat("agentHeight"))
		{
			mImpl->mAgentHeight = *agentHeight;
		}

		Matrix4DRow wt;
		wt = Matrix4DRow::CreateScale(mImpl->mGridSize * 2.0f);
		wt *= Matrix4DRow::CreateTranslation(GetWorldPosition());

		mOwner->GetSceneContext()->GetRenderer()->GetDebugRenderer()->AddPersistantDebugBox({ wt, Vector3D(0.0, 0.0, 1.0) });
		mImpl->mWorldTransformBackup = wt;
	}

	void NavGridVolumeComponent::Serialize(Serialization::ISerializer& pWriter)
	{
		Component::BeginSerialize(pWriter);
		pWriter.WriteVector3D("gridSize", mImpl->mGridSize);
		pWriter.WriteVector2D("nbOfPoints", mImpl->mNumberOfPoint);
		pWriter.WriteFloat("agentWidth", mImpl->mAgentWidth);
		pWriter.WriteFloat("agentHeight", mImpl->mAgentHeight);
		Component::EndSerialize(pWriter);
	}

	std::vector<PropertyDescriptor> NavGridVolumeComponent::GetProperties()
	{
		std::function<void()> computeCallback = [&]()
			{
				mOwner->GetSceneContext()->GetNavGridManager()->ComputeGrid();
			};

		const bool condition = mImpl->mGridSize.x > 0 && mImpl->mGridSize.y > 0 && mImpl->mAgentWidth > 0 && mImpl->mAgentHeight > 0;
		
		Matrix4DRow wt;

		wt = Matrix4DRow::CreateScale(mImpl->mGridSize * 2.0f);
		wt *= Matrix4DRow::CreateTranslation(GetWorldPosition());

		if (wt != mImpl->mWorldTransformBackup)
		{
			mOwner->GetSceneContext()->GetRenderer()->GetDebugRenderer()->RemovePersistantDebugBox(mImpl->mWorldTransformBackup);
			mOwner->GetSceneContext()->GetRenderer()->GetDebugRenderer()->AddPersistantDebugBox({ wt, Vector3D(0.0, 0.0, 1.0) });
			mImpl->mWorldTransformBackup = wt;
		}

		return
		{
			{ "Grid Size : ", &mImpl->mGridSize, PropertyType::Vec3},
			{ "Points Quantity : ", &mImpl->mNumberOfPoint, PropertyType::Vec2},
			{ "Agent Width : ", &mImpl->mAgentWidth, PropertyType::Float},
			{ "Agent Height : ", &mImpl->mAgentHeight, PropertyType::Float},
			{ "Compute Grid : ", nullptr, PropertyType::Button, Callback(computeCallback, "Compute") | Condition(condition)},
			{ "Show line trace : ", &mImpl->mShowLines, PropertyType::Bool},
			{ "Show position : ", &mImpl->mShowNodePos, PropertyType::Bool},
			{ "Show Agent loc : ", &mImpl->mShowAgentCollision, PropertyType::Bool},
		};
	}

	void NavGridVolumeComponent::OnStart()
	{
		
	}

	Vector3D NavGridVolumeComponent::GetGridSize() const { return mImpl->mGridSize; }
	Vector2D NavGridVolumeComponent::GetAgentSize() const { return Vector2D{ mImpl->mAgentWidth, mImpl->mAgentHeight }; }
	float NavGridVolumeComponent::GetAgentWidth() const { return mImpl->mAgentWidth; }
	float NavGridVolumeComponent::GetAgentHeight() const { return mImpl->mAgentHeight; }
	Vector2D NavGridVolumeComponent::GetNumberOfPoints() const { return mImpl->mNumberOfPoint; }

	bool NavGridVolumeComponent::GetShowLines() const { return mImpl->mShowLines; }
	bool NavGridVolumeComponent::GetShowNodePosition() const { return mImpl->mShowNodePos; }
	bool NavGridVolumeComponent::GetShowAgentCollision() const { return mImpl->mShowAgentCollision; }
}
