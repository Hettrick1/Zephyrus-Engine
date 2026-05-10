#include "pch.h"
#include "AiControllerComponent.h"
#include "AI/NavGridManager.h"
#include "DebugRenderer.h"
#include "Timer.h"

namespace Zephyrus::ActorComponent
{
	struct AiControllerComponent::Impl
	{
		AiControllerComponent::Impl() = default;

		std::optional<Vector3D> mTarget = std::nullopt;

		std::vector<AI::GridNode*> mPath;
		AI::GridNode* mCurrentNodeTarget = nullptr;
		unsigned mNodeIndex = 0;

		bool mRecomputePath = false;
		bool mShouldMove = true;
	};

	AiControllerComponent::AiControllerComponent(Actor* pOwner)
		: Component(pOwner, "AiControllerComponent"), mImpl(std::make_unique<Impl>())
	{
	}

	AiControllerComponent::~AiControllerComponent()
	{
	}

	void AiControllerComponent::Deserialize(Serialization::IDeserializer& pReader)
	{
		Component::Deserialize(pReader);
	}

	void AiControllerComponent::Serialize(Serialization::ISerializer& pWriter)
	{
		Component::BeginSerialize(pWriter);
		Component::EndSerialize(pWriter);
	}

	std::vector<PropertyDescriptor> AiControllerComponent::GetProperties()
	{
		return {};
	}

	void AiControllerComponent::OnStart()
	{
		mOwner->AddTag("AI");
	}

	void AiControllerComponent::Update()
	{
		if (mImpl->mTarget)
		{
			if (mImpl->mRecomputePath)
			{
				mImpl->mPath.clear();
				mImpl->mCurrentNodeTarget = nullptr;
				mImpl->mNodeIndex = 0;
				mOwner->GetSceneContext()->GetRenderer()->GetDebugRenderer()->FlushDebugArrows(2);
				auto startNode = mOwner->GetSceneContext()->GetNavGridManager()->GetNearestNodeFromWorldPosition(mOwner->GetPosition());
				auto endNode = mOwner->GetSceneContext()->GetNavGridManager()->GetNearestNodeFromWorldPosition(mImpl->mTarget.value());
				if (startNode == endNode || !startNode || !endNode)
				{
					mImpl->mShouldMove = false;
					return;
				}
				mImpl->mPath = mOwner->GetSceneContext()->GetNavGridManager()->GetShortestPath(startNode, endNode);
				for (auto node : mImpl->mPath)
				{
					if (node->parent) // starting from the second node
					{
						auto line = Debug::Debug2DArrow(node->parent->nodePosition.AddZ(0.2f), node->nodePosition.AddZ(0.2f));
						mOwner->GetSceneContext()->GetRenderer()->GetDebugRenderer()->AddDebugArrow(line, 2);
					}
				}

				if (mImpl->mPath.empty())
					return;
				mImpl->mCurrentNodeTarget = mImpl->mPath[mImpl->mNodeIndex];
				mImpl->mShouldMove = true;
				mImpl->mRecomputePath = false;
			}

			if (mImpl->mPath.empty())
				return;

			if (mImpl->mCurrentNodeTarget && mImpl->mShouldMove)
			{
				auto direction = mImpl->mCurrentNodeTarget->nodePosition - mOwner->GetPosition();
				direction.Normalize();

				mOwner->GetTransformComponent().Translate(direction * 2.0f * Timer::deltaTime);

				if (mImpl->mCurrentNodeTarget->nodePosition.NearlyEquals(mOwner->GetPosition(), 0.01f))
				{
					if (mImpl->mCurrentNodeTarget != mImpl->mPath.back())
					{
						mOwner->GetSceneContext()->GetRenderer()->GetDebugRenderer()->FlushDebugLines(5);
						mImpl->mNodeIndex++;
						mImpl->mCurrentNodeTarget = mImpl->mPath[mImpl->mNodeIndex];
						auto line = Debug::DebugLine(mImpl->mCurrentNodeTarget->nodePosition.AddZ(0.6f), mImpl->mCurrentNodeTarget->nodePosition, {}, Vector3D(1, 1, 0));
						mOwner->GetSceneContext()->GetRenderer()->GetDebugRenderer()->AddDebugLine(line, 5);
						return;
					}		
					if (mImpl->mCurrentNodeTarget->nodePosition.NearlyEquals(mOwner->GetPosition(), 0.01f))
					{
						mImpl->mShouldMove = false;
					}
				}
			}
		}
	}

	void AiControllerComponent::SetTarget(const Vector3D& target)
	{
		mImpl->mTarget = target;
		mImpl->mRecomputePath = true;
	}

	void AiControllerComponent::ResetTarget()
	{
		mImpl->mTarget = std::nullopt;
		mImpl->mPath.clear();
	}


}
