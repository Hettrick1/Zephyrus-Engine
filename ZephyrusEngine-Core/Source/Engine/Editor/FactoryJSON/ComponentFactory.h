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

    bool Register(const std::string& pName, Creator pCreateFn);

    Component* Create(const std::string& pName, Actor* pOwner);

private:
    std::unordered_map<std::string, Creator> mCreators;
};