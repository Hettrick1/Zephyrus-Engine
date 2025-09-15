#pragma once
#include "PropertyDescriptor.h"

#include "../EditorApplication/EventSystem/EventSystem.h"
#include "../EditorApplication/EventSystem/Event/SetGenericPropertyEvent.h"
#include "Component.h"

#include <functional>
#include <string>

struct Property {
    std::string name;
    PropertyType type;

    std::function<void(void*)> setter;
    std::function<void* ()> getter;
};

template<typename T>
Property MakeUndoableProperty(const PropertyDescriptor& desc, Component* owner) {
    if constexpr (std::is_pointer_v<T>) 
    {
        // For T = Mesh*, Texture*, ...
        T** field = static_cast<T**>(desc.field);

        return {
            desc.name,
            desc.type,
            [field, owner](void* val)
            {
                T* oldVal = *field;
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
            [field, owner](void* val)
            {
                T oldVal = *field;
                T newVal = *static_cast<T*>(val);

                auto* evt = new SetGenericPropertyEvent<T>(owner, field, oldVal, newVal);
                EventSystem::DoEvent(evt);
            },
            [field]() -> void* { return field; }
        };
    }
}