#include "ComponentRegister.h"
#include "ComponentFactory.h"
#include "SkySphereComponent.h"
#include "Temp/DoomComponents/DoomMenuPC.h"
#include "MeshComponent.h"
#include "SpriteComponent.h"
#include "Temp/DoomComponents/PickUpComponent.h"
#include "Temp/DoomComponents/DoomPC.h"
#include "Temp/DoomComponents/DoomPlayerComponent.h"
#include "FlipbookComponent.h"
#include "Temp/DoomComponents/DoomEnemyComponent.h"
#include "PlayerStartComponent.h"
#include "Physics/Bullet/BulletRigidbodyComponent.h"
#include "Physics/Bullet/CubeColliderComponent.h"
#include "Physics/Bullet/SphereColliderComponent.h"
#include "Physics/Bullet/CapsuleColliderComponent.h"
#include "CameraComponent.h"

void ComponentRegister::RegisterAllComponents()
{
	ComponentFactory::Instance().Register("SkySphereComponent", Zephyrus::ActorComponent::SkySphereComponent::Create);
	ComponentFactory::Instance().Register("DoomMenuPC", Zephyrus::ActorComponent::DoomMenuPC::Create);
	ComponentFactory::Instance().Register("MeshComponent", Zephyrus::ActorComponent::MeshComponent::Create);
	ComponentFactory::Instance().Register("SpriteComponent", Zephyrus::ActorComponent::SpriteComponent::Create);
	ComponentFactory::Instance().Register("PickUpComponent", Zephyrus::ActorComponent::PickUpComponent::Create);
	ComponentFactory::Instance().Register("DoomPC", Zephyrus::ActorComponent::DoomPC::Create);
	ComponentFactory::Instance().Register("DoomPlayerComponent", Zephyrus::ActorComponent::DoomPlayerComponent::Create);
	ComponentFactory::Instance().Register("FlipbookComponent", Zephyrus::ActorComponent::FlipbookComponent::Create);
	ComponentFactory::Instance().Register("DoomEnemyComponent", Zephyrus::ActorComponent::DoomEnemyComponent::Create);
	ComponentFactory::Instance().Register("PlayerStartComponent", Zephyrus::ActorComponent::PlayerStartComponent::Create);
	ComponentFactory::Instance().Register("RigidbodyComponent", Zephyrus::ActorComponent::BulletRigidbodyComponent::Create);
	ComponentFactory::Instance().Register("CubeColliderComponent", Zephyrus::ActorComponent::CubeColliderComponent::Create);
	ComponentFactory::Instance().Register("SphereColliderComponent", Zephyrus::ActorComponent::SphereColliderComponent::Create);
	ComponentFactory::Instance().Register("CapsuleColliderComponent", Zephyrus::ActorComponent::CapsuleColliderComponent::Create);
	ComponentFactory::Instance().Register("CameraComponent", Zephyrus::ActorComponent::CameraComponent::Create);
}
