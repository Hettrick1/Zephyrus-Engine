#include "BoxSATComponent.h"
#include "Matrix4DRow.h"
#include "IRenderer.h"
#include "Physics/ContactManifold.h"

BoxSATComponent::BoxSATComponent(Actor* owner, int updateOrder, Vector3D size, Vector3D relativePosition)
    :ColliderComponent(owner, updateOrder)
{
    SetRelativePosition(relativePosition);
    mSize = size;
}

bool BoxSATComponent::CheckCollisionWith(ColliderComponent* other, ContactManifold& infosOut)
{
    if (other->GetColliderType() == ColliderType::BoxSAT) {
        return CheckCollisionWithBoxSAT(static_cast<BoxSATComponent*>(other), infosOut);
    }
    return false;
}

AABB BoxSATComponent::GetAABB()
{
    AABB aabb = AABB(GetWorldPosition() - mSize, GetWorldPosition() + mSize);
    return aabb;
}

void BoxSATComponent::Update()
{
}

std::vector<Vector3D> BoxSATComponent::GetVertices()
{
    std::vector<Vector3D> vertices;
    Vector3D halfSize = GetScaledSize();
    Matrix4DRow transform = GetWorldTransform();

    // Génération des 8 sommets avec la transformation complète
    for (float x : {-1.0f, 1.0f}) {
        for (float y : {-1.0f, 1.0f}) {
            for (float z : {-1.0f, 1.0f}) {
                Vector3D vertex = Vector3D(x, y, z) * halfSize;
                vertex = transform.TransformVector(vertex);
                vertices.push_back(vertex);
            }
        }
    }
    return vertices;
}

void BoxSATComponent::DebugDraw(IRenderer& renderer)
{
    if (mOwner->GetState() == ActorState::Active)
    {
        AABB aabb = AABB(-mSize, mSize);

        Matrix4DRow wt = Matrix4DRow::CreateScale(mSize * 2.0f);
        //wt *= Matrix4DRow::CreateFromQuaternion(mOwner->GetTransformComponent().GetRotation());
        wt *= Matrix4DRow::CreateTranslation(GetWorldPosition() - mSize);

        renderer.DrawDebugBox(aabb.min, aabb.max, wt);
    }
}

bool BoxSATComponent::CheckCollisionWithBoxSAT(BoxSATComponent* other, ContactManifold& infosOut)
{
    Vector3D axesA[3], axesB[3];
    GetAxes(axesA);
    other->GetAxes(axesB);

    Vector3D centerA = GetWorldPosition();
    Vector3D centerB = other->GetWorldPosition();
    Vector3D t = centerB - centerA;

    float minOverlap = FLT_MAX;
    Vector3D smallestAxis;

    // --- Check axes of A
    for (int i = 0; i < 3; i++) {
        Vector3D axis = axesA[i];
        if (axis.LengthSq() < 0.001f) continue; // éviter des axes nuls

        float overlap;
        if (!OverlapOnAxis(other, axis, overlap))
            return false;

        if (overlap < minOverlap) {
            minOverlap = overlap;
            smallestAxis = axis;
        }
    }

    // --- Check axes of B
    for (int i = 0; i < 3; i++) {
        Vector3D axis = axesB[i];
        if (axis.LengthSq() < 0.001f) continue;

        float overlap;
        if (!OverlapOnAxis(other, axis, overlap))
            return false;

        if (overlap < minOverlap) {
            minOverlap = overlap;
            smallestAxis = axis;
        }
    }

    // --- Check cross products
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            Vector3D axis = Vector3D::Cross(axesA[i], axesB[j]);
            if (axis.LengthSq() < 0.001f) continue;

            axis = Vector3D::Normalize(axis);

            float overlap;
            if (!OverlapOnAxis(other, axis, overlap))
                return false;

            if (overlap < minOverlap) {
                minOverlap = overlap;
                smallestAxis = axis;
            }
        }
    }

    centerA = GetWorldPosition();
    centerB = other->GetWorldPosition();
    Vector3D direction = centerB - centerA;

    Vector3D newDir = Vector3D(direction.x, direction.y, 0);

    if (Vector3D::Dot(smallestAxis, direction) < 0.0f) {
        smallestAxis = -smallestAxis;
    }

    Vector3D finalNormal = Vector3D::Normalize((smallestAxis + Vector3D::Normalize(direction)));

    Vector3D newNorm = Vector3D(finalNormal.x, finalNormal.y, 0); // Normale en 2D car je n'ai pas implémenté la gravité encore donc ca évite que des quilles s'envolent

    infosOut.normal = Vector3D::Normalize(newNorm);
    infosOut.penetrationDepth = minOverlap;

    return true;
}

bool BoxSATComponent::OverlapOnAxis(BoxSATComponent* other, const Vector3D& axis, float& overlapOut) {
    auto projA = CalculateProjection(axis);
    auto projB = other->CalculateProjection(axis);

    if (projA.second < projB.first || projB.second < projA.first)
        return false;

    // Calcul du overlap réel
    overlapOut = std::min(projA.second, projB.second) - std::max(projA.first, projB.first);
    return true;
}

void BoxSATComponent::GetAxes(Vector3D axes[3])
{
    Matrix4DRow transform = GetWorldTransform();
    axes[0] = Vector3D::Normalize(transform.GetXAxis()); // Right
    axes[1] = Vector3D::Normalize(transform.GetYAxis()); // Forward
    axes[2] = Vector3D::Normalize(transform.GetZAxis()); // Up
}

std::pair<float, float> BoxSATComponent::CalculateProjection(const Vector3D& axis)
{
    Vector3D axes[3];
    GetAxes(axes);
    Vector3D scaledExtents = GetScaledSize();

    float proj = Vector3D::Dot(GetWorldPosition(), axis);
    float radius =
        scaledExtents.x * std::abs(Vector3D::Dot(axes[0], axis)) +
        scaledExtents.y * std::abs(Vector3D::Dot(axes[1], axis)) +
        scaledExtents.z * std::abs(Vector3D::Dot(axes[2], axis));

    return { proj - radius, proj + radius };
}

Vector3D BoxSATComponent::GetScaledSize() {
    Vector3D scale = mOwner->GetTransformComponent().GetSize();
    return Vector3D(
        mSize.x * std::abs(scale.x),
        mSize.y * std::abs(scale.y),
        mSize.z * std::abs(scale.z)
    );
}