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
    BoxSATComponent(Actor* owner, int updateOrder, Vector3D size = 1, Vector3D relativePosition = Vector3D::zero);

    ColliderType GetColliderType() const override { return ColliderType::BoxSAT; }
    bool CheckCollisionWith(ColliderComponent* other, ContactManifold& infosOut) override;
    AABB GetAABB() override;
    void Update() override;
    void DebugDraw(IRenderer& renderer) override;

    std::vector<Vector3D> GetVertices();

private : 
    bool CheckCollisionWithBoxSAT(BoxSATComponent* other, ContactManifold& infosOut);
    void GetAxes(Vector3D axes[3]);
    bool OverlapOnAxis(BoxSATComponent* other, const Vector3D& axis, float& overlapOut);
    std::pair<float, float> CalculateProjection(const Vector3D& axis);
    Vector3D GetScaledSize();
};