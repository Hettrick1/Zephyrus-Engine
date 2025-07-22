#pragma once
#include "ColliderComponent.h"
#include "Physics/AABB.h"
#include "Quaternion.h"

/**
 * @brief Collider component representing an oriented box using the Separating Axis Theorem (SAT) for collision detection.
 * Handles collision checks, bounding box calculation, and debug drawing for box colliders.
 * 
 * INFO : Still work in progress
 */
class BoxSATComponent : public ColliderComponent {
public :
    BoxSATComponent() = delete;
    BoxSATComponent(Actor* pOwner, int pUpdateOrder, Vector3D pSize = 1, Vector3D pRelativePosition = Vector3D::zero);

    ColliderType GetColliderType() const override { return ColliderType::BoxSAT; }
    bool CheckCollisionWith(ColliderComponent* pOther, ContactManifold& pInfosOut) override;
    AABB GetAABB() override;
    void Update() override;
    void DebugDraw(IRenderer& pRenderer) override;

    std::vector<Vector3D> GetVertices();

private : 
    bool CheckCollisionWithBoxSAT(BoxSATComponent* pOther, ContactManifold& pInfosOut);
    void GetAxes(Vector3D pAxes[3]);
    bool OverlapOnAxis(BoxSATComponent* pOther, const Vector3D& pAxis, float& pOverlapOut);
    std::pair<float, float> CalculateProjection(const Vector3D& pAxis);
    Vector3D GetScaledSize();
};