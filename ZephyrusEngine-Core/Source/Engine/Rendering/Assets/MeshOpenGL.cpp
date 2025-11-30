#include "pch.h"
#include "MeshOpenGL.h"

namespace Zephyrus::Render {

    MeshOpenGL::MeshOpenGL(const Assets::MeshData& data)
    {
        mMeshPath = data.sourceFile;
        auto verts = data.ToVerticeArray();
        mVertexArray = std::make_unique<Assets::VertexArrayOpenGL>(verts.data(), verts.size());
    }

    MeshOpenGL::~MeshOpenGL()
    {
    }

    void MeshOpenGL::Bind() const
    {
        mVertexArray->SetActive();
    }

    void MeshOpenGL::Unbind() const
    {
        glBindVertexArray(0);
    }

}
