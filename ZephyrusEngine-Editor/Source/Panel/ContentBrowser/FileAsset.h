#pragma once

#include <imgui.h>
#include <string>
#include "AssetsManager.h"
#include "Log.h"

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

    bool Rename(const std::string& newName)
    {
        auto db= Zephyrus::Assets::AssetsManager::GetInstance().GetDatabase();

        std::string nameWithExtension = newName + mExtension;
        
        std::filesystem::path newPath = std::filesystem::path(mPath);
        std::filesystem::path newMetaPath = std::filesystem::path(mMetaPath);
        newPath.replace_filename(nameWithExtension);
        newMetaPath = newPath.string() + ".meta";
        ZP_EDITOR_INFO("Renamed " + mId + newPath.string());
        db.UpdatePathFromID(mId, newPath.string());
        std::filesystem::rename(mPath, newPath);
        std::filesystem::rename(mMetaPath, newMetaPath);
        mPath = newPath.string();
        mMetaPath = newMetaPath.string();
        return true;
    }
    bool Move(const std::string& newPath)
    {
        auto db= Zephyrus::Assets::AssetsManager::GetInstance().GetDatabase();
        return false;
    }
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

