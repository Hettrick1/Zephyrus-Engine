#include "FrameUboOpenGL.h"

#include  "glew.h"

namespace Zephyrus::Render
{
    FrameUboOpenGL::~FrameUboOpenGL()
    {
        if (mBuffer)
        {
            glDeleteBuffers(1, &mBuffer);
        }
    }


    void FrameUboOpenGL::Initialize()
    {
        glGenBuffers(1, &mBuffer);
        glBindBuffer(GL_UNIFORM_BUFFER, mBuffer);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(FrameData), nullptr, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);


        // Bind to slot 0
        glBindBufferBase(GL_UNIFORM_BUFFER, 0, mBuffer);
    }


    void FrameUboOpenGL::UpdateData(const FrameData& data)
    {
        glBindBuffer(GL_UNIFORM_BUFFER, mBuffer);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(FrameData), &data);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }
}