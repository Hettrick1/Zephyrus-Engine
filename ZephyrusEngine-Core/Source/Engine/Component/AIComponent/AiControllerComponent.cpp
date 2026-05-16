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

		float mLastDistance = 0.0f;
		bool mNeedPreciseMovements = false;
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
				mOwner->GetSceneContext()->GetRenderer()->GetDebugRenderer()->FlushDebugBoxes(10);
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

				mOwner->GetSceneContext()->GetNavGridManager()->SmoothPath(mImpl->mPath);

				if (mImpl->mPath.empty())
				{
					ZP_WARN("Path Empty");
					return;
				}

				for (auto node : mImpl->mPath)
				{
					Debug::DebugBox box = Debug::DebugBox(node->nodePosition, Vector3D(0.25), {}, Vector3D(1.0, 0.5, 0.0));
					mOwner->GetSceneContext()->GetRenderer()->GetDebugRenderer()->AddDebugBox(box, 10);
				}

				mImpl->mCurrentNodeTarget = mImpl->mPath[mImpl->mNodeIndex];
				if (mImpl->mPath.size() > 1)
				{
					const auto dir1 = mImpl->mPath[mImpl->mNodeIndex + 1]->nodePosition.RemoveZ() - mImpl->mCurrentNodeTarget->nodePosition.RemoveZ();
					const auto dir2 = mOwner->GetPosition().RemoveZ() - mImpl->mCurrentNodeTarget->nodePosition.RemoveZ();
					if (Vector3D::Dot(dir1, dir2) > 0.0)
					{
						mImpl->mCurrentNodeTarget = mImpl->mPath[mImpl->mNodeIndex + 1];
					}
				}
				mImpl->mLastDistance = mOwner->GetPosition().DistanceSquared(mImpl->mCurrentNodeTarget->nodePosition);
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
				if (!rb)
				{
					return;
				}

				if (mImpl->mShouldMove) // chut on ne regarde pas cette implementation hasardeuse qui date
				{
					btRigidBody* body = rb->GetRigidBody();
					body->activate(true);

					btVector3 desiredDir = btVector3(direction.x, direction.y, 0).normalize();

					btVector3 slopeDir = desiredDir - (desiredDir.dot(Physics::ToBtVec3(groundNormal)) * Physics::ToBtVec3(groundNormal));
					slopeDir.normalize();

					float speed = 5.0f;
					btVector3 targetVelocity = slopeDir * speed;

					btVector3 currentVel = body->getLinearVelocity();

					float accelerationTime = 0.1f;
					btVector3 steeringForce = (targetVelocity - currentVel) / accelerationTime;

					float maxForce = 50.0f;
					if (steeringForce.length2() > maxForce * maxForce) 
					{
						steeringForce = steeringForce.normalize() * maxForce;
					}

					body->applyCentralForce(steeringForce);
				}

				float distance = mImpl->mCurrentNodeTarget->nodePosition.DistanceSquared(hit.HitPoint);
				float factor = 0.35f;

				bool goNext = mImpl->mNeedPreciseMovements ? mImpl->mCurrentNodeTarget->nodePosition.NearlyEquals(hit.HitPoint, 0.1f) : distance < mImpl->mLastDistance * factor;

				if (mImpl->mCurrentNodeTarget != mImpl->mPath.back() && mImpl->mCurrentNodeTarget->nodePosition.NearlyEquals(hit.HitPoint, 0.1f))
				{
					mOwner->GetSceneContext()->GetRenderer()->GetDebugRenderer()->FlushDebugLines(5);
					mImpl->mNodeIndex++;

					auto nextNode = mImpl->mPath[mImpl->mNodeIndex];
					auto nextNextNode = nextNode;
					if (mImpl->mNodeIndex + 1 < mImpl->mPath.size())
					{
						nextNextNode = mImpl->mPath[mImpl->mNodeIndex + 1];
					}

					if (nextNextNode->nodePosition.z > hit.HitPoint.z)
					{
						mImpl->mNeedPreciseMovements = true;
					}
					else
					{
						mImpl->mNeedPreciseMovements = false;
					}
					mImpl->mCurrentNodeTarget = nextNode;
					mImpl->mLastDistance = mImpl->mCurrentNodeTarget->nodePosition.DistanceSquared(hit.HitPoint);
					auto line = Debug::DebugLine(mImpl->mCurrentNodeTarget->nodePosition.AddZ(0.6f), mImpl->mCurrentNodeTarget->nodePosition, {}, Vector3D(1, 1, 0));
					mOwner->GetSceneContext()->GetRenderer()->GetDebugRenderer()->AddDebugLine(line, 5);
					return;	
				}
				else if (mImpl->mCurrentNodeTarget == mImpl->mPath.back() && mImpl->mCurrentNodeTarget->nodePosition.NearlyEquals(hit.HitPoint, 0.5f))
				{
					mImpl->mShouldMove = false;
					btRigidBody* body = rb->GetRigidBody();
					body->activate(true);
					body->setLinearVelocity(btVector3(0, 0, 0));
					body->activate(false);
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
