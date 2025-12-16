#pragma once
#include "FramePass.h"

class GeometryPass : public FramePass
{
public:
    GeometryPass(const std::string& name);
    ~GeometryPass() override = default;
    
    void Execute(PassContext& ctx) override;
};
