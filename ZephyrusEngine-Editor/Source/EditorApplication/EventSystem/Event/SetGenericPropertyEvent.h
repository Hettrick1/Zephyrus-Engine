#pragma once

#include "Event.h"
#include "Component.h"

template<typename T>
class SetGenericPropertyEvent : public Event
{
private:
    Component* mComponent = nullptr;
    T* mField = nullptr;
    T mOldValue;
    T mNewValue;

public:
    SetGenericPropertyEvent(Component* component, T* field, const T oldValue, const T newValue)
        : Event("Set Property"), mComponent(component), mField(field), mOldValue(oldValue), mNewValue(newValue)
    {
    }
    ~SetGenericPropertyEvent()
    {
        mComponent = nullptr;
        mField = nullptr;
    }
    void Execute() override
    {
        if (mField)
        {
            *mField = mNewValue;
        }
    }
    void Undo() override
    {
        if (mField)
        {
            *mField = mOldValue;
        }
    }
};