#pragma once

namespace Zephyrus::Render
{
    class IRenderer;
}

struct PassContext
{
    Zephyrus::Render::IRenderer* renderer{ nullptr };
};
