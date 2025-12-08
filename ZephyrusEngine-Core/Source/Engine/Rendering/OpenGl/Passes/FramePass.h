#pragma once
#include "../TextureHandle.h"
#include "PassContext.h"

class FramePass
{
protected:
    std::string mName;
    std::vector<TextureHandle> mReads;
    std::vector<TextureHandle> mWrites;
public:
    FramePass(const std::string& name);
    virtual ~FramePass() = default;

    void Read(const TextureHandle& textureHandle) {mReads.push_back(textureHandle);}
    void Write(const TextureHandle& textureHandle) {mWrites.push_back(textureHandle);}
    
    virtual void SetUp(PassContext& ctx);
    virtual void Execute(PassContext& ctx);
    virtual void CleanUp(PassContext& ctx);
};
