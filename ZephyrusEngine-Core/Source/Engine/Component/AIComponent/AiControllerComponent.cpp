#include "pch.h"
#include "AiControllerComponent.h"
#include "AI/NavGridManager.h"
#include "DebugRenderer.h"
#include "Physics/Bullet/PhysicWorld.h"
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
				auto startNode = mOwner->GetSceneContext()->GetNavGridManager()->GetNearestNodeFromWorldPosition(mOwner->GetPosition());
				auto endNode = mOwner->GetSceneContext()->GetNavGridManager()->GetNearestNodeFromWorldPosition(mImpl->mTarget.value());
				if (startNode == endNode || !startNode || !endNode)
				{
					mImpl->mShouldMove = false;
					return;
				}
				mImpl->mPath = mOwner->GetSceneContext()->GetNavGridManager()->GetShortestPath(startNode, endNode);

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

				auto rb = mOwner->GetRigidBody();
				if (rb && rb->GetRigidBody())
				{
					btRigidBody* body = rb->GetRigidBody();
					body->activate(true);

					btVector3 vel = body->getLinearVelocity();
					btVector3 target(direction.x * 10.0f, direction.y * 10.0f, vel.z());
					float factor = 0.3f;
					btVector3 smoothVel = vel.lerp(target, factor);
					body->setLinearVelocity(smoothVel);
				}

				//mOwner->GetTransformComponent().Translate(direction * 2.0f * Timer::deltaTime);

				auto startPos = mOwner->GetPosition();
				auto endPos = mOwner->GetPosition().AddZ(-mOwner->GetSize().z);
				HitResult hit;
				mOwner->GetSceneContext()->GetPhysicsWorld()->LineTrace(startPos, endPos, hit, {mOwner});

				if (mImpl->mCurrentNodeTarget->nodePosition.NearlyEquals(hit.HitPoint, 0.01f))
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
