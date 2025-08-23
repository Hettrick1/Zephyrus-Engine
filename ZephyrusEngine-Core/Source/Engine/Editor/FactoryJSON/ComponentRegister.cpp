#include "ComponentRegister.h"
#include "ComponentFactory.h"
#include "CameraComponent.h"
#include "SkySphereComponent.h"
#include "DoomMenuPC.h"
#include "MeshComponent.h"
#include "BoxAABBComponent.h"

void ComponentRegister::RegisterAllComponents()
{
	ComponentFactory::Instance().Register("CameraComponent", CameraComponent::Create);
	ComponentFactory::Instance().Register("SkySphereComponent", SkySphereComponent::Create);
	ComponentFactory::Instance().Register("DoomMenuPC", DoomMenuPC::Create);
	ComponentFactory::Instance().Register("MeshComponent", MeshComponent::Create);
	ComponentFactory::Instance().Register("BoxAABBComponent", BoxAABBComponent::Create);
}
