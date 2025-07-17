#include "BoxAABBComponent.h"
#include "MeshComponent.h"
#include "IRenderer.h"
#include "Assets.h"
#include <algorithm>
#include <iostream>

BoxAABBComponent::BoxAABBComponent(Actor* pOwner, int pUpdateOder, Vector3D pSize, Vector3D pRelativePosition)
    : ColliderComponent(pOwner, pUpdateOder), mShowInGame(true)
{
    //pOwner->AddComponent(this);
    SetRelativePosition(pRelativePosition);
    mPosition = pOwner->GetTransformComponent().GetPosition() + GetRelativePosition(); 
    mLastPosition = mPosition;
    mSize = pSize;
}

BoxAABBComponent::~BoxAABBComponent()
{
}

void BoxAABBComponent::OnStart()
{
}

void BoxAABBComponent::Update()
{
    ColliderComponent::Update();
    mPosition = mOwner->GetTransformComponent().GetPosition() + GetRelativePosition();
}

void BoxAABBComponent::OnEnd()
{
}

bool BoxAABBComponent::CheckCollisionWith(ColliderComponent* other, ContactManifold& infosOut)
{
    if (BoxAABBComponent* boxCollider = dynamic_cast<BoxAABBComponent*>(other)) {
        if (CheckCollisionWithBoxAABB(boxCollider, infosOut)) {
            return true;
        }
        return false;
    }
    return false;
}

bool BoxAABBComponent::CheckCollisionWithBoxAABB(BoxAABBComponent* other, ContactManifold& infosOut)
{
    mLastPosition = mPosition;
    mPosition = mOwner->GetTransformComponent().GetPosition() + GetRelativePosition();

    Vector3D delta = mPosition - mLastPosition;
    Vector3D otherDelta = (other->GetOwner()->GetTransformComponent().GetPosition() + other->GetRelativePosition()) - other->GetLastPosition();

    float maxDelta = std::max(delta.Length(), otherDelta.Length());
    int steps = std::max(1, static_cast<int>(maxDelta / 0.05f));
    float stepSize = 1.0f / steps;

    for (int i = 0; i <= steps; i++)
    {
        float t = i * stepSize;
        Vector3D predictedPosition = mLastPosition + delta * t;
        Vector3D otherPredictedPosition = other->GetLastPosition() + otherDelta * t;

        Vector3D minA = predictedPosition - (mSize);
        Vector3D maxA = predictedPosition + (mSize);
        Vector3D minB = otherPredictedPosition - (other->mSize);
        Vector3D maxB = otherPredictedPosition + (other->mSize);

        if ((minA.x <= maxB.x && maxA.x >= minB.x) &&
            (minA.y <= maxB.y && maxA.y >= minB.y) &&
            (minA.z <= maxB.z && maxA.z >= minB.z))
        {
            mCollisionPosition.first = predictedPosition;
            mCollisionPosition.second = otherPredictedPosition;
            return true;
        }
    }

    // Pas de collision, reset des valeurs
    mCollisionPosition.first = Vector3D(0, 0, 0);
    mCollisionPosition.second = Vector3D(0, 0, 0);
    return false;
}

void BoxAABBComponent::SetShowInGame(bool pShowInGame)
{
}

void BoxAABBComponent::SetSize(Vector3D pSize)
{
	mSize = pSize;
}

void BoxAABBComponent::DebugDraw(IRenderer& renderer)
{
    if (mOwner->GetState() == ActorState::Active)
    {
        AABB aabb = AABB(mPosition - mSize, mPosition + mSize);

        Matrix4DRow wt;

        wt = Matrix4DRow::CreateScale(mSize * 2);
        wt *= Matrix4DRow::CreateTranslation(mPosition - mSize);

        renderer.DrawDebugBox(aabb.min, aabb.max, wt);
    }
}

Vector3D BoxAABBComponent::GetLastPosition()
{
    return mLastPosition;
}

AABB BoxAABBComponent::GetAABB()
{
    return AABB(mPosition - mSize, mPosition + mSize);
}
