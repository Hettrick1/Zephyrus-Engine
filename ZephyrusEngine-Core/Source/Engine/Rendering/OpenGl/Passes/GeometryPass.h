#pragma once
#include "FramePass.h"

class GeometryPass : public FramePass
{
public:
    GeometryPass(const std::string& name);
    ~GeometryPass() override = default;
    
    void SetUp(PassContext& ctx) override;
    void Execute(PassContext& ctx) override;
    void CleanUp(PassContext& ctx) override;
};
