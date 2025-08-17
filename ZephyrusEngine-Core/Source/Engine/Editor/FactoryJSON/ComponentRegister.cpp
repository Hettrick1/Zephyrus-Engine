#include "ComponentRegister.h"
#include "ComponentFactory.h"
#include "CameraComponent.h"
#include "SkySphereComponent.h"
#include "DoomMenuPC.h"

void ComponentRegister::RegisterAllComponents()
{
	ComponentFactory::Instance().Register("CameraComponent", CameraComponent::Create);
	ComponentFactory::Instance().Register("SkySphereComponent", SkySphereComponent::Create);
	ComponentFactory::Instance().Register("DoomMenuPC", DoomMenuPC::Create);
}
