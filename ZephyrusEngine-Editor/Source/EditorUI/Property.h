#pragma once
#include "Property/PropertyDescriptor.h"

#include "../EditorApplication/EventSystem/EventSystem.h"
#include "../EditorApplication/EventSystem/Event/SetGenericPropertyEvent.h"
#include "Component.h"

#include <functional>
#include <string>

struct Property {
    std::string name;
    PropertyType type;

    std::function<void(void*, void*)> setter;
    std::function<void* ()> getter;

    void Set(void* val) { setter(val, nullptr); }
    void Set(void* val, void* old) { setter(val, old); }
};

template<typename T>
Property MakeUndoableProperty(const PropertyDescriptor& desc, Zephyrus::ActorComponent::Component* owner) {
    if constexpr (std::is_pointer_v<T>) 
    {
        // For T = Mesh*, Texture*, ...
        T** field = static_cast<T**>(desc.field);

        return {
            desc.name,
            desc.type,
            [field, owner](void* val, void* old)
            {
                T* oldVal = nullptr;
                if (old)
                {
                    oldVal = static_cast<T*>(old);
                }
                else
                {
                    oldVal = *field;
                }

                T* newVal = static_cast<T*>(val);

                auto* evt = new SetGenericPropertyEvent<T*>(owner, field, oldVal, newVal);
                EventSystem::DoEvent(evt);
            },
            [field]() -> void* { return *field; }
        };
    }
    else 
    {
        // For float, bool, ...
        T* field = static_cast<T*>(desc.field);

        return {
            desc.name,
            desc.type,
            [field, owner](void* val, void* old)
            {
                T oldVal = *field;
                if (old)
                {
                    oldVal = *static_cast<T*>(old);
                }
                T newVal = *static_cast<T*>(val);

                auto* evt = new SetGenericPropertyEvent<T>(owner, field, oldVal, newVal);
                EventSystem::DoEvent(evt);
            },
            [field]() -> void* { return field; }
        };
    }
}