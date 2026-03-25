#pragma once

#include <string>

enum class FileType
{
    None,
    Map,
    Prefab,
    Image,
    Font,
    Mesh,
    Material,
    Shader,
};

struct FileAsset
{
    std::string mId;
    std::string mPath;
    std::string mMetaPath;
    std::string mExtension;
    FileType mType;
};

inline FileType GetTypeFromExtension(const std::string& extension)
{
    if (extension == ".png" || extension == ".jpg" || extension == ".jpeg")
    {
        return FileType::Image;
    }
    if (extension == ".otf" || extension == ".ttf")
    {
        return FileType::Font;
    }
    if (extension == ".zpmap")
    {
        return FileType::Map;
    }
    if (extension == ".obj" || extension == ".fbx")
    {
        return FileType::Mesh;
    }
    if (extension == ".prefab")
    {
        return FileType::Prefab;
    }
    if (extension == ".vert" || extension == ".frag" || extension == ".tesc" || extension == ".tese" || extension == ".geom")
    {
        return FileType::Shader;
    }
    if (extension == ".zpmat")
    {
        return FileType::Material;
    }
    
    return FileType::None;
}

