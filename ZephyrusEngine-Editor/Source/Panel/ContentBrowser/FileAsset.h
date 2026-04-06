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
    std::string mFileName;
    std::string mExtension;
    FileType mType;

    bool Rename(const std::string& newName)
    {
        auto db= Zephyrus::Assets::AssetsManager::GetInstance().GetFileDatabase();

        std::string nameWithExtension = newName + mExtension;
        
        std::filesystem::path newPath = std::filesystem::path(mPath);
        std::filesystem::path newMetaPath = std::filesystem::path(mMetaPath);
        newPath.replace_filename(nameWithExtension);
        newMetaPath = newPath.string() + ".meta";

        if (std::filesystem::exists(newMetaPath) && std::filesystem::exists(newPath))
        {
            ZP_EDITOR_INFO("Cannot rename, name already taken : " + mId + " - " + newPath.string());
            return false;
        }
        
        ZP_EDITOR_INFO("Renamed " + mId + " - to - "+ newPath.string());
        db.UpdatePathFromID(mId, newPath.string());
        std::filesystem::rename(mPath, newPath);
        std::filesystem::rename(mMetaPath, newMetaPath);
        mPath = newPath.string();
        mMetaPath = newMetaPath.string();
        mFileName = newPath.stem().string();
        return true;
    }
    bool Move(const std::string& newPath)
    {
        auto db= Zephyrus::Assets::AssetsManager::GetInstance().GetFileDatabase();
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

inline std::string GetStringFromFileType(const FileType& type)
{
    switch (type)
    {
        case FileType::None:
        return "Undefined";
        case FileType::Map:
        return "Map";
        case FileType::Prefab:
        return "Prefab";
        case FileType::Image:
        return "Texture";
        case FileType::Font:
        return "Font";
        case FileType::Mesh:
        return "Mesh";
        case FileType::Material:
        return "Material";
        case FileType::Shader:
        return "Shader";
    }
    return "Undefined";
}

