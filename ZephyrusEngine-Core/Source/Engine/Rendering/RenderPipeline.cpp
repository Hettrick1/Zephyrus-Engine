#include "pch.h"
#include "RenderPipeline.h"

#include "Passes/FramePass.h"

RenderPipeline::RenderPipeline(Zephyrus::Render::IRenderer* renderer)
{
    mContext.renderer = renderer;
}

void RenderPipeline::SetUp()
{
    for (auto pass : passes)
    {
        pass->SetUp(mContext);
    }
}

void RenderPipeline::ExecutePasses()
{
    for (auto pass : passes)
    {
        pass->Execute(mContext);
    }
}
