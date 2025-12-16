#include "pch.h"
#include "SkyBoxPass.h"

SkyBoxPass::SkyBoxPass(const std::string& name)
    : FramePass(name)
{
}

void SkyBoxPass::Execute(PassContext& ctx)
{
    FramePass::Execute(ctx);
    
}

