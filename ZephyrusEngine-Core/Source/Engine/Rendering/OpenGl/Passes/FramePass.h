#pragma once
#include "../TextureHandle.h"
#include "PassContext.h"

class FramePass
{
protected:
    std::string mName;
public:
    FramePass(const std::string& name);
    virtual ~FramePass() = default;
    
    virtual void Execute(PassContext& ctx);
};
