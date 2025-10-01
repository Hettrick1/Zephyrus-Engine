#pragma once

#include "Component.h"
#include <unordered_map>
#include <string>
#include <functional>

class ComponentFactory
{
public:
	using Creator = std::function<Zephyrus::ActorComponent::Component* (Zephyrus::ActorComponent::Actor*)>;

	static ComponentFactory& Instance();

    bool Register(const std::string& pName, Creator pCreateFn);

    Zephyrus::ActorComponent::Component* Create(const std::string& pName, Zephyrus::ActorComponent::Actor* pOwner);

    inline std::vector<std::string> GetComponentNames() const { return mComponentNames; }

private:
    std::unordered_map<std::string, Creator> mCreators;
    std::vector<std::string> mComponentNames;
};