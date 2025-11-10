#pragma once

#include "Event.h"
#include "Component.h"
#include <functional>

template<typename T>
class SetGenericPropertyEvent : public Event
{
public:
    using SetterFunc = std::function<void(T)>;

private:
    Zephyrus::ActorComponent::Component* mOwner;
    T* mField = nullptr;
    T mOldValue;
    T mNewValue;
    SetterFunc mSetter = nullptr;

public:
    SetGenericPropertyEvent(Zephyrus::ActorComponent::Component* owner, T* field, T oldValue, T newValue)
        : Event("Set property"), mOwner(owner), mField(field), mOldValue(oldValue), mNewValue(newValue)
    {
    }

    SetGenericPropertyEvent(Zephyrus::ActorComponent::Component* owner, T oldValue, T newValue, SetterFunc setter)
        : Event("Set property"), mOwner(owner), mField(nullptr), mOldValue(oldValue), mNewValue(newValue), mSetter(setter)
    {
    }

    void Execute() override
    {
        if (mSetter)
            mSetter(mNewValue);
        else if (mField)
            *mField = mNewValue;
    }

    void Undo() override
    {
        if (mSetter)
            mSetter(mOldValue);
        else if (mField)
            *mField = mOldValue;
    }
};