#include "ComponentFactory.h"

namespace Zephyrus::Factory {
    ComponentFactory& ComponentFactory::Instance()
    {
        static ComponentFactory instance;
        return instance;
    }

    bool ComponentFactory::Register(const std::string& pName, Creator pCreateFn)
    {
        mComponentNames.push_back(pName);
        return mCreators.emplace(pName, pCreateFn).second;
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