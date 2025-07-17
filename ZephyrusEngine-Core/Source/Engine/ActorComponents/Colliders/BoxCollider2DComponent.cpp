#include "BoxCollider2DComponent.h"
#include "SpriteComponent.h"
#include "Scene.h"
#include "Actor.h"

BoxCollider2DComponent::BoxCollider2DComponent(Actor* pOwner, int pUpdateOder, Vector3D pSize) 
    : ColliderComponent(pOwner, pUpdateOder), mSize(pSize), mShowInGame(true)
{
    mPosition = pOwner->GetTransformComponent().GetPosition();
    if (mShowInGame) {
        Texture* tex = Assets::LoadTexture(*mOwner->GetScene().GetRenderer(), "Imports/Sprites/collider.png", "ground");
        SpriteComponent* newSprite = new SpriteComponent(mOwner, *tex, 4000, pSize);
        mOwner->AddComponent(newSprite);
    }
}

BoxCollider2DComponent::~BoxCollider2DComponent()
{
}

void BoxCollider2DComponent::OnStart()
{
}

void BoxCollider2DComponent::Update()
{
    mPosition = mOwner->GetTransformComponent().GetPosition();
}

void BoxCollider2DComponent::OnEnd()
{
}

bool BoxCollider2DComponent::CheckCollisionWith(ColliderComponent* other, ContactManifold& infosOut)
{
    if (BoxCollider2DComponent* boxCollider = dynamic_cast<BoxCollider2DComponent*>(other)) {
        if (CheckCollisionWithBox(boxCollider)) {
            return true;
        }
        return false;
    }
    return false;
}

bool BoxCollider2DComponent::CheckCollisionWithBox(BoxCollider2DComponent* other)
{
    float left1 = mPosition.x - mSize.x / 2;
    float right1 = mPosition.x + mSize.x / 2;
    float top1 = mPosition.y - mSize.y / 2;
    float bottom1 = mPosition.y + mSize.y / 2;

    float left2 = other->mPosition.x - other->mSize.x / 2;
    float right2 = other->mPosition.x + other->mSize.x / 2;
    float top2 = other->mPosition.y - other->mSize.y / 2;
    float bottom2 = other->mPosition.y + other->mSize.y / 2;

    bool xOverlap = (left1 < right2) && (right1 > left2);
    bool yOverlap = (top1 < bottom2) && (bottom1 > top2);


    return xOverlap && yOverlap;
}

void BoxCollider2DComponent::SetShowInGame(bool showInGame)
{
    mShowInGame = showInGame;
}

void BoxCollider2DComponent::SetSize(Vector3D pSize)
{
    mSize = pSize;
}

