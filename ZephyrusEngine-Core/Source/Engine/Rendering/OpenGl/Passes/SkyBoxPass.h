#pragma once
#include "FramePass.h"

class SkyBoxPass : public FramePass
{
public:
    SkyBoxPass(const std::string& name);
    ~SkyBoxPass() override = default;
    
    void Execute(PassContext& ctx) override;
};
