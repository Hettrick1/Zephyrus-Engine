#pragma once
#include <vector>
#include "Vertex.h"
#include <string>

namespace Zephyrus::Assets {
    struct MeshData
    {
        std::vector<Zephyrus::Render::Vertex> vertices;
        std::vector<uint32_t> indices;
        std::string sourceFile;

        std::vector<float> ToVerticeArray() const
        {
            std::vector<float> array;
            array.reserve(vertices.size() * 8);
            for (auto& v : vertices)
            {
                array.push_back(v.position.x);
                array.push_back(v.position.z);
                array.push_back(v.position.y);

                array.push_back(v.normal.x);
                array.push_back(v.normal.z);
                array.push_back(v.normal.y);

                array.push_back(v.texCoord.x);
                array.push_back(-v.texCoord.y);
            }
            return array;
        }
    };
}