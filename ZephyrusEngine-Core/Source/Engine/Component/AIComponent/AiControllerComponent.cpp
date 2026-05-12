#include "pch.h"
#include "AiControllerComponent.h"
#include "AI/NavGridManager.h"
#include "DebugRenderer.h"
#include "Physics/Bullet/PhysicWorld.h"
#include "Physics/Bullet/bulletConversion.h"
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

				Vector3D groundNormal(0, 0, 1);

				auto startPos = mOwner->GetPosition();
				auto endPos = mOwner->GetPosition().AddZ(-mOwner->GetSize().z);
				HitResult hit;
				mOwner->GetSceneContext()->GetPhysicsWorld()->LineTrace(startPos, endPos, hit, { mOwner });
				auto line = Debug::DebugLine(startPos, endPos, {}, Vector3D(1, 0, 0));
				mOwner->GetSceneContext()->GetRenderer()->GetDebugRenderer()->AddDebugLine(line, 5);

				if (hit.HasHit) {
					groundNormal = hit.Normal;
				}

				auto rb = mOwner->GetRigidBody();
				if (rb && mImpl->mShouldMove) // chut on ne regarde pas cette implementation hasardeuse qui date
				{
					btRigidBody* body = rb->GetRigidBody();
					body->activate(true);

					float maxSpeed = 5.0f;
					btVector3 desiredDir = btVector3(direction.x, direction.y, 0).normalize();

					btVector3 slopeDir = desiredDir - (desiredDir.dot(Physics::ToBtVec3(groundNormal)) * Physics::ToBtVec3(groundNormal));
					slopeDir.normalize();

					float speed = 5.0f;
					btVector3 targetVelocity = slopeDir * speed;

					btVector3 currentVel = body->getLinearVelocity();

					float accelerationTime = 0.1f;
					btVector3 steeringForce = (targetVelocity - currentVel) / accelerationTime;

					float maxForce = 50.0f;
					if (steeringForce.length2() > maxForce * maxForce) {
						steeringForce = steeringForce.normalize() * maxForce;
					}

					body->applyCentralForce(steeringForce);
				}

				if (mImpl->mCurrentNodeTarget->nodePosition.NearlyEquals(hit.HitPoint, 0.1f))
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
					if (mImpl->mCurrentNodeTarget->nodePosition.NearlyEquals(hit.HitPoint, 0.3f))
					{
						mImpl->mShouldMove = false;
						btRigidBody* body = rb->GetRigidBody();
						body->activate(true);
						body->setLinearVelocity(btVector3(0, 0, 0));
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
