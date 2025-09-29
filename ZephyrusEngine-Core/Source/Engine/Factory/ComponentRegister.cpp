#include "ComponentRegister.h"
#include "ComponentFactory.h"
#include "CameraComponent.h"
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
#include "NewCameraComponent.h"

void ComponentRegister::RegisterAllComponents()
{
	ComponentFactory::Instance().Register("CameraComponent", CameraComponent::Create);
	ComponentFactory::Instance().Register("SkySphereComponent", SkySphereComponent::Create);
	ComponentFactory::Instance().Register("DoomMenuPC", DoomMenuPC::Create);
	ComponentFactory::Instance().Register("MeshComponent", MeshComponent::Create);
	ComponentFactory::Instance().Register("SpriteComponent", SpriteComponent::Create);
	ComponentFactory::Instance().Register("PickUpComponent", PickUpComponent::Create);
	ComponentFactory::Instance().Register("DoomPC", DoomPC::Create);
	ComponentFactory::Instance().Register("DoomPlayerComponent", DoomPlayerComponent::Create);
	ComponentFactory::Instance().Register("FlipbookComponent", FlipbookComponent::Create);
	ComponentFactory::Instance().Register("DoomEnemyComponent", DoomEnemyComponent::Create);
	ComponentFactory::Instance().Register("PlayerStartComponent", PlayerStartComponent::Create);
	ComponentFactory::Instance().Register("RigidbodyComponent", BulletRigidbodyComponent::Create);
	ComponentFactory::Instance().Register("CubeColliderComponent", CubeColliderComponent::Create);
	ComponentFactory::Instance().Register("SphereColliderComponent", SphereColliderComponent::Create);
	ComponentFactory::Instance().Register("CapsuleColliderComponent", CapsuleColliderComponent::Create);
	ComponentFactory::Instance().Register("NewCameraComponent", NewCameraComponent::Create);
}
