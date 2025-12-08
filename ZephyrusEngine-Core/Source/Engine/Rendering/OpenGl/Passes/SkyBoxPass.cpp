#include "pch.h"
#include "SkyBoxPass.h"

SkyBoxPass::SkyBoxPass(const std::string& name)
    : FramePass(name)
{
}

void SkyBoxPass::SetUp(PassContext& ctx)
{
    FramePass::SetUp(ctx);

}

void SkyBoxPass::Execute(PassContext& ctx)
{
    FramePass::Execute(ctx);
    
}

void SkyBoxPass::CleanUp(PassContext& ctx)
{
    FramePass::CleanUp(ctx);
}
