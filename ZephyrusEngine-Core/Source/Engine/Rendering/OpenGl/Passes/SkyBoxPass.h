#pragma once
#include "FramePass.h"

class SkyBoxPass : public FramePass
{
public:
    SkyBoxPass(const std::string& name);
    ~SkyBoxPass() override = default;
    
    void SetUp(PassContext& ctx) override;
    void Execute(PassContext& ctx) override;
    void CleanUp(PassContext& ctx) override;
};
