#pragma once

#include "PropertyFlags.h"
#include <string>

namespace Zephyrus
{
    struct PropertyMetadata
    {
        uint16_t flags = 0;

        // optional metadata
        float minFValue = 0.0f;
        float maxFValue = 0.0f;

        int minIValue = 0;
        int maxIValue = 0;

        bool hasRange  = false;
        bool hasStep   = false;

        std::string tooltip;
        std::string displayName;
        std::string category;
    };
    
    // Meta data templates
    inline PropertyMetadata Range(float min, float max)
    {
        PropertyMetadata m;
        m.flags |= PropertyFlags::Range;
        m.minFValue = min;
        m.maxFValue = max;
        m.hasRange = true;
        return m;
    }
    
    inline PropertyMetadata Range(int min, int max)
    {
        PropertyMetadata m;
        m.flags |= PropertyFlags::Range;
        m.minIValue = min;
        m.maxIValue = max;
        m.hasRange = true;
        return m;
    }
    
    inline PropertyMetadata Clamp(float min, float max)
    {
        PropertyMetadata m;
        m.flags |= PropertyFlags::Clamp;
        m.minFValue = min;
        m.maxFValue = max;
        m.hasRange = true;
        return m;
    }
    
    inline PropertyMetadata ReadOnly()
    {
        PropertyMetadata m;
        m.flags |= PropertyFlags::Read_Only;
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

}
