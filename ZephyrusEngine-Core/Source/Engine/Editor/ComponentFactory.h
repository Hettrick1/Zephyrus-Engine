#pragma once

#include "Component.h"
#include <unordered_map>
#include <string>
#include <functional>

class ComponentFactory
{
public:
	using Creator = std::function<Component* (Actor*)>;

	static ComponentFactory& Instance();

    bool Register(const std::string& name, Creator createFn) {
        return mCreators.emplace(name, createFn).second;
    }

    Component* Create(const std::string& pName, Actor* pOwner);

private:
    std::unordered_map<std::string, Creator> mCreators;
};