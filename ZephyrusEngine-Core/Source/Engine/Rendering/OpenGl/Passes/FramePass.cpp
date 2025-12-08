#include "pch.h"
#include "FramePass.h"

FramePass::FramePass(const std::string& name)
    : mName(name)
{
}

void FramePass::Execute(PassContext& ctx)
{
    for (int i = 0; i < mReads.size(); i++)
    {
        glBindTexture(mReads[i].mHandle, i);
    }
}
