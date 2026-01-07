#pragma once

#include <string>
#include "AttachementFormat.h"
#include <unordered_map>
#include <memory>

struct AttachementDesc
{
    AttachementDesc(const std::string& pName, FramebufferTextureFormat pFormat)
        : mName{ pName }, mFormat{ pFormat }
    {
        
    }
    std::string mName;
    FramebufferTextureFormat mFormat {FramebufferTextureFormat::None};
};

class IFrameBuffer
{
public:
    virtual ~IFrameBuffer() = default;

    virtual void Bind() = 0;
    virtual void UnBind() = 0;

    virtual unsigned int GetHandle() const = 0;
    
    virtual unsigned int GetColorAttachement(const std::string& name) = 0;
    virtual void AddColorAttachement(AttachementDesc descriptor) = 0;
    virtual void AddDepthAttachement(FramebufferTextureFormat format) = 0;
    virtual void RemoveColorAttachement(const std::string& name) = 0;
    virtual unsigned int GetDepthAttachement() = 0;
    
    virtual void Init() = 0;
    virtual void Destroy() = 0;
    
    virtual void Resize(int pWidth, int pHeight) = 0;
    virtual Vector2D GetDimensions() const = 0;
};
