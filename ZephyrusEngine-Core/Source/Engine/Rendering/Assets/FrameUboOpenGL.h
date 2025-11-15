#pragma once

#include "Data/FrameData.h"

namespace Zephyrus::Render
{
    class FrameUboOpenGL
    {
    public:
        FrameUboOpenGL() = default;
        ~FrameUboOpenGL();


        void Initialize();
        void UpdateData(const FrameData& data);


        unsigned int GetBuffer() const { return mBuffer; }


    private:
        unsigned int mBuffer{ 0 };
    };
}

