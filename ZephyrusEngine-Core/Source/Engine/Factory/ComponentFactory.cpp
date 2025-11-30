#include "pch.h"
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
#include "AtmosphereComponent.h"

using Zephyrus::ActorComponent::SkySphereComponent;
using Zephyrus::ActorComponent::DoomMenuPC;
using Zephyrus::ActorComponent::MeshComponent;
using Zephyrus::ActorComponent::SpriteComponent;
using Zephyrus::ActorComponent::PickUpComponent;
using Zephyrus::ActorComponent::DoomPC;
using Zephyrus::ActorComponent::DoomPlayerComponent;
using Zephyrus::ActorComponent::FlipbookComponent;
using Zephyrus::ActorComponent::DoomEnemyComponent;
using Zephyrus::ActorComponent::PlayerStartComponent;
using Zephyrus::ActorComponent::BulletRigidbodyComponent;
using Zephyrus::ActorComponent::CubeColliderComponent;
using Zephyrus::ActorComponent::SphereColliderComponent;
using Zephyrus::ActorComponent::CapsuleColliderComponent;
using Zephyrus::ActorComponent::CameraComponent;
using Zephyrus::ActorComponent::AtmosphereComponent;

namespace Zephyrus::Factory {
	ComponentFactory::ComponentFactory()
	{
		RegisterAllComponents();
	}

	bool ComponentFactory::Register(const std::string& pName, Creator pCreateFn)
    {
        mComponentNames.push_back(pName);
        return mCreators.emplace(pName, pCreateFn).second;
    }

    void ComponentFactory::RegisterAllComponents()
    {
	    Register("SkySphereComponent", SkySphereComponent::Create);
		Register("DoomMenuPC", DoomMenuPC::Create);
		Register("MeshComponent", MeshComponent::Create);
		Register("SpriteComponent", SpriteComponent::Create);
		Register("PickUpComponent", PickUpComponent::Create);
		Register("DoomPC", DoomPC::Create);
		Register("DoomPlayerComponent", DoomPlayerComponent::Create);
		Register("FlipbookComponent", FlipbookComponent::Create);
		Register("DoomEnemyComponent", DoomEnemyComponent::Create);
		Register("PlayerStartComponent", PlayerStartComponent::Create);
		Register("RigidbodyComponent", BulletRigidbodyComponent::Create);
		Register("CubeColliderComponent", CubeColliderComponent::Create);
		Register("SphereColliderComponent", SphereColliderComponent::Create);
		Register("CapsuleColliderComponent", CapsuleColliderComponent::Create);
		Register("CameraComponent", CameraComponent::Create);
		Register("AtmosphereComponent", AtmosphereComponent::Create);
    }

    Component* ComponentFactory::Create(const std::string& pName, Actor* pOwner)
    {
        auto it = mCreators.find(pName);
        if (it != mCreators.end()) {
            return it->second(pOwner);
        }
        return nullptr;
    }
}