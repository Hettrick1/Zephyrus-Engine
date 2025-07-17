#include "CollisionResolver.h"
#include "RigidbodyComponent.h"

CollisionResolver::~CollisionResolver()
{
	for (auto& pair : mRigidbodies)
	{
		RigidbodyComponent* rb = pair.second;
		if (rb != nullptr)
		{
			delete rb;
		}
		rb = nullptr;
	}
	mRigidbodies.clear();
	for (auto& pair : mQuerryCollisions)
	{
		CollisionInfos* ci = pair.second;
		if (ci != nullptr)
		{
			delete ci;
		}
		ci = nullptr;
	}
	mQuerryCollisions.clear();
	for (auto& pair : mPhysicCollisions)
	{
		CollisionInfos* ci = pair.second;
		if (ci != nullptr)
		{
			delete ci;
		}
		ci = nullptr;
	}
	mPhysicCollisions.clear();
}

void CollisionResolver::RegisterRigidBody(Actor* pOwner, RigidbodyComponent* pRigidbody)
{
	if (mRigidbodies.find(pOwner) == mRigidbodies.end())
	{
		mRigidbodies[pOwner] = pRigidbody;
	}
	else
	{
		Log::Error(LogType::System, "You already have a rigidbody attached to this actor !");
	}
}

void CollisionResolver::RemoveRigidBody(Actor* pOwner, RigidbodyComponent* pRigidbody)
{
	auto it = mRigidbodies.find(pOwner);
	if (it != mRigidbodies.end())
	{
		mRigidbodies.erase(it);
	}
}

void CollisionResolver::Unload()
{
	for (auto& pair : mQuerryCollisions)
	{
		CollisionInfos* ci = pair.second;
		if (ci != nullptr)
		{
			delete ci;
		}
		ci = nullptr;
	}
	mQuerryCollisions.clear();
	for (auto& pair : mPhysicCollisions)
	{
		CollisionInfos* ci = pair.second;
		if (ci != nullptr)
		{
			delete ci;
		}
		ci = nullptr;
	}
	mPhysicCollisions.clear();
	mRigidbodies.clear();
	mCollisionPosition.clear();
	mReactionForce.clear();
}

void CollisionResolver::UpdateRigidbodies()
{
	if (mRigidbodies.empty())
	{
		return;
	}
	for (auto it = mRigidbodies.begin(); it != mRigidbodies.end(); it++)
	{
		RigidbodyComponent* rb = it->second;
		if (rb != nullptr)
		{
			rb->Update();
		}
	}
}

void CollisionResolver::AddCollisionToQueue(CollisionInfos* pCollisionInfo)
{
	std::pair<ColliderComponent*, ColliderComponent*> colliders = pCollisionInfo->colliderPair;
	std::pair<Actor*, Actor*> actors = pCollisionInfo->actorPair;
	if (colliders.first->GetIsQuerry() || colliders.second->GetIsQuerry())
	{
		if (mQuerryCollisions.find(colliders) == mQuerryCollisions.end())
		{
			mQuerryCollisions[colliders] = pCollisionInfo;
		}
	}
	else {
		if (mPhysicCollisions.find(actors) == mPhysicCollisions.end())
		{
			mPhysicCollisions[actors] = pCollisionInfo;
		}
	}
}

void CollisionResolver::ResolveCollisions()
{
	if (!mPhysicCollisions.empty())
	{
		CalculatePhysicCollisions();
		mPhysicCollisions.clear();
	}
	if (!mQuerryCollisions.empty())
	{
		CalculateQuerryCollisions();
		mQuerryCollisions.clear();
	}
}

void CollisionResolver::CalculateQuerryCollisions()
{
	for (auto it = mQuerryCollisions.begin(); it != mQuerryCollisions.end(); it++)
	{
		CollisionInfos* collision = it->second;
		HitResult hit1 = HitResult();
		hit1.HitActor = collision->actorPair.first;
		HitResult hit2 = HitResult();
		hit2.HitActor = collision->actorPair.second;
		std::pair<ColliderComponent*, ColliderComponent*> colliders = collision->colliderPair;
		switch (collision->type)
		{
		case CollisionType::Enter:
		{
			colliders.first->NotifyListenersStarted(&hit2);
			colliders.second->NotifyListenersStarted(&hit1);
			break;
		}
		case CollisionType::Stay:
		{
			colliders.first->NotifyListenersStay(&hit2);
			colliders.second->NotifyListenersStay(&hit1);
			break;
		}
		case CollisionType::Exit:
		{
			colliders.first->NotifyListenersEnded(&hit2);
			colliders.second->NotifyListenersEnded(&hit1);
			break;
		}
		default:
		{
			Log::Error(LogType::Error, "The collision type is needed for querry collisions !");
			break;
		}
		}
	}
}

void CollisionResolver::CalculatePhysicCollisions()
{
	for (auto it = mPhysicCollisions.begin(); it != mPhysicCollisions.end(); it++)
	{
		CollisionInfos* collision = it->second;

		std::pair<Actor*, Actor*> actors = collision->actorPair;

		RigidbodyComponent* rbA = actors.first->GetRigidBody(); 
		RigidbodyComponent* rbB = actors.second->GetRigidBody();

		Vector3D vA = collision->velocityPair.first;
		Vector3D vB = collision->velocityPair.second;

		bool isStaticA = (rbA && rbA->IsStatic()) || (rbA && rbA->GetMass() > 10000);
		bool isStaticB = (rbB && rbB->IsStatic()) || (rbB && rbB->GetMass() > 10000);

		bool isGroundedA = ((rbA && rbA->IsStatic()) || (rbA && rbA->GetMass() > 10000) || (rbA && rbA->GetIsGrounded()));
		bool isGroundedB = ((rbB && rbB->IsStatic()) || (rbB && rbB->GetMass() > 10000) || (rbB && rbB->GetIsGrounded()));

		Vector3D colPos1 = collision->positionPair.first;
		Vector3D colPos2 = collision->positionPair.second;

		float penetrationDepth = collision->depth;

		Vector3D normal = collision->normal;
		normal = Vector3D::Normalize(normal);

		if (!rbA && !rbB)
		{
			continue;
		}
		switch (collision->type)
		{
			case CollisionType::Enter:
			{
				ColliderComponent* colliderA = collision->colliderPair.first;
				ColliderComponent* colliderB = collision->colliderPair.second;

				if (isGroundedA && normal.z > 0.1)
				{
					rbB->SetIsGrounded(true);
				}
				if (isGroundedB && normal.z > 0.1)
				{
					rbA->SetIsGrounded(true);
				}

				Vector3D velocityA = (rbA && !isStaticA) ? vA : Vector3D(0);
				Vector3D velocityB = (rbB && !isStaticB) ? vB : Vector3D(0);

				Vector3D relativeVelocity = velocityA - velocityB;
				float vRel = Vector3D::Dot(relativeVelocity, normal);

				float e = (rbA ? rbA->GetBounciness() : 0.0f) * (rbB ? rbB->GetBounciness() : 0.0f);
				float invMassA = (rbA && rbA->GetMass() > 0.0f) ? 1.0f / rbA->GetMass() : 0.0f;
				float invMassB = (rbB && rbB->GetMass() > 0.0f) ? 1.0f / rbB->GetMass() : 0.0f;

				float j = -(1 + e) * vRel / (invMassA + invMassB);
				j *= 1.3f;
				Vector3D impulse = j * normal;

				if (rbA && !isStaticA)
				{
					mReactionForce[rbA] = impulse * invMassA;
				}
				if (rbB && !isStaticB)
				{
					mReactionForce[rbB] = (impulse*-1) * invMassB;
				}
				break;
			}
			case CollisionType::Stay:
			{
				ResolvePenetration(actors.first, actors.second, normal, penetrationDepth);
				break;
			}
			case CollisionType::Exit:
			{
				if (isGroundedA && normal.z > 0.1)
				{
					rbB->SetIsGrounded(false);
				}
				if (isGroundedB && normal.z > 0.1)
				{
					rbA->SetIsGrounded(false);
				}
				break;
			}
			default:
			{
				Log::Error(LogType::Error, "The collision type is needed for physic collisions !");
				break;
			}
		}
	}
	mPhysicCollisions.clear();
	ApplyReactionForce();
}

void CollisionResolver::ApplyReactionForce()
{
	for (auto it = mReactionForce.begin(); it != mReactionForce.end(); it++)
	{
		it->first->ResolveCollision(it->second);
	}
	mReactionForce.clear();
}

void CollisionResolver::ResolvePenetration(Actor* actorA, Actor* actorB, Vector3D normal, float depth)
{
	RigidbodyComponent* rbA = actorA->GetRigidBody();
	RigidbodyComponent* rbB = actorB->GetRigidBody();

	bool isStaticA = (rbA && rbA->IsStatic()) || (rbA && rbA->GetMass() > 10000);
	bool isStaticB = (rbB && rbB->IsStatic()) || (rbB && rbB->GetMass() > 10000);

	if (isStaticA && isStaticB) return;

	float totalMass = 0.0f;
	if (rbA && !isStaticA) totalMass += rbA->GetMass();
	if (rbB && !isStaticB) totalMass += rbB->GetMass();

	if (totalMass <= 0.0f) return;

	float percentA = 0;
	float percentB = 0;

	if (rbA)
	{
		percentA = isStaticA ? 0.0f : rbA->GetMass() / totalMass;
	}
	if (rbB)
	{
		percentB = isStaticB ? 0.0f : rbB->GetMass() / totalMass;
	}
	depth *= 1.01f;

	if (!isStaticA && rbA) {
		Vector3D movement = normal * (-depth * (1.0f - percentA));
		Vector3D newPos = actorA->GetTransformComponent().GetPosition() + movement;
		actorA->SetPosition(newPos);
	}

	if (!isStaticB && rbB) {
		Vector3D movement = normal * (depth * (1.0f - percentB));
		Vector3D newPos = actorB->GetTransformComponent().GetPosition() + movement;
		actorB->SetPosition(newPos);
	}
}
