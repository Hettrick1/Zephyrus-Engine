#pragma once
#include "FramePass.h"

class SpritePass : public FramePass
{
public:
    SpritePass(const std::string& name);
    ~SpritePass() override = default;
    
    void Execute(PassContext& ctx) override;
};
