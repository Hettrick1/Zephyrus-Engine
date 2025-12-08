#pragma once
#include "FramePass.h"

class SpritePass : public FramePass
{
public:
    SpritePass(const std::string& name);
    ~SpritePass() override = default;
    
    void SetUp(PassContext& ctx) override;
    void Execute(PassContext& ctx) override;
    void CleanUp(PassContext& ctx) override;
};
