#pragma once

#include "PropertyFlags.h"
#include <string>

namespace Zephyrus
{
    struct PropertyMetadata
    {
        uint16_t flags = 0;
        
        float minFValue = 0.0f;
        float maxFValue = 0.0f;

        int minIValue = 0;
        int maxIValue = 0;

        bool hasFRange  = false;
        bool hasIRange  = false;
        bool hasStep   = false;
        
        bool condition = false;

        std::string tooltip;
        std::string displayName;
        std::string category;
    };

    inline PropertyMetadata operator|(PropertyMetadata a, const PropertyMetadata& b) {

        a.flags |= b.flags;
        
        if (b.hasFRange)
        {
            a.minFValue = b.minFValue;
            a.maxFValue = b.maxFValue;
            a.hasFRange  = true;
        }
        
        if (b.hasIRange)
        {
            a.minIValue = b.minIValue;
            a.maxIValue = b.maxIValue;
            a.hasIRange = true;
        }

        if (b.hasStep)
            a.hasStep = true;
        
        if (b.condition)
            a.condition = true;

        if (!b.tooltip.empty())
            a.tooltip = b.tooltip;
        
        if (!b.displayName.empty())
            a.displayName = b.displayName;

        if (!b.category.empty())
            a.category = b.category;

        return a;
    }
    
    // Meta data templates
    inline PropertyMetadata Range(float min, float max)
    {
        PropertyMetadata m;
        m.flags |= PropertyFlags::Range;
        m.minFValue = min;
        m.maxFValue = max;
        m.hasFRange = true;
        return m;
    }
    
    inline PropertyMetadata Range(int min, int max)
    {
        PropertyMetadata m;
        m.flags |= PropertyFlags::Range;
        m.minIValue = min;
        m.maxIValue = max;
        m.hasIRange = true;
        return m;
    }
    
    inline PropertyMetadata ReadOnly()
    {
        PropertyMetadata m;
        m.flags |= PropertyFlags::Disable_In_Editor;
        return m;
    }
    
    inline PropertyMetadata Tooltip(const std::string& text)
    {
        PropertyMetadata m;
        m.flags |= PropertyFlags::Tooltip;
        m.tooltip = text;
        return m;
    }
    
    inline PropertyMetadata DisplayName(const std::string& name)
    {
        PropertyMetadata m;
        m.flags |= PropertyFlags::DisplayName;
        m.displayName = name;
        return m;
    }
    
    inline PropertyMetadata Category(const std::string& cat)
    {
        PropertyMetadata m;
        m.flags |= PropertyFlags::Category;
        m.category = cat;
        return m;
    }

    inline PropertyMetadata Condition(const bool condition, PropertyFlags::Flags showBehavior = PropertyFlags::Disable_In_Editor)
    {
        PropertyMetadata m;
        m.flags |= PropertyFlags::Condition;
        m.flags |= showBehavior;
        m.condition = condition;
        return m;
    }

}
