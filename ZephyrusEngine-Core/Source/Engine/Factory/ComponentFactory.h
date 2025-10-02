#pragma once

#include "Component.h"
#include <unordered_map>
#include <string>
#include <functional>

using Zephyrus::ActorComponent::Actor;
using Zephyrus::ActorComponent::Component;

class ComponentFactory
{
public:
	using Creator = std::function<Component* (Actor*)>;

	static ComponentFactory& Instance();

    bool Register(const std::string& pName, Creator pCreateFn);

    Component* Create(const std::string& pName, Actor* pOwner);

    inline std::vector<std::string> GetComponentNames() const { return mComponentNames; }

private:
    std::unordered_map<std::string, Creator> mCreators;
    std::vector<std::string> mComponentNames;
};