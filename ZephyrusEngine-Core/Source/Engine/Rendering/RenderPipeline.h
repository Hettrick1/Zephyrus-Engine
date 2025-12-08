#pragma once
#include "Actor.h"
#include "PassContext.h"

class FramePass;

class RenderPipeline
{
    std::vector<std::shared_ptr<FramePass>> passes;
    PassContext mContext;
    
public:
    RenderPipeline(Zephyrus::Render::IRenderer* renderer);
    void SetUp();
    void ExecutePasses();
};
