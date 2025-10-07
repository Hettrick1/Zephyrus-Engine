#pragma once
#include "../Interface/IMesh.h"
#include "../Data/MeshData.h"
#include "VertexArray.h"
#include <glew.h>
#include <memory>

namespace Zephyrus::Render {

    class MeshOpenGL : public Assets::IMesh
    {
    private:
        std::unique_ptr<Zephyrus::Assets::VertexArrayOpenGL> mVertexArray;
        std::string mMeshPath;
    public:
        explicit MeshOpenGL(const Assets::MeshData& data);
        ~MeshOpenGL();

        void Bind() const override;
        void Unbind() const override;
        uint32_t GetVertexCount() const override { return mVertexArray->GetVerticeCount(); }
        std::string GetFilePath() const override { return mMeshPath; }
    };

}