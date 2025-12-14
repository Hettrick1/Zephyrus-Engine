#pragma once

#include "TextureHandle.h"
#include <string>
#include <unordered_map>

class IFrameBuffer
{
public:
    virtual ~IFrameBuffer() = default;

    virtual void Bind() = 0;
    virtual void UnBind() = 0;

    virtual void GetHandle() = 0;

    virtual std::unordered_map<std::string, TextureHandle*> GetAllColorAttachements() = 0;
    virtual TextureHandle* GetColorAttachement(const std::string& name) = 0;
    virtual void AddColorAttachement(const std::string& name) = 0;
    virtual void RemoveColorAttachement(const std::string& name) = 0;
    virtual unsigned int GetDepthAttachement() = 0;
    
    virtual void Init() = 0;
    virtual void Destroy() = 0;
    
    virtual void Resize(int pWidth, int pHeight) = 0;
    virtual Vector2D GetDimensions() const = 0;
};
