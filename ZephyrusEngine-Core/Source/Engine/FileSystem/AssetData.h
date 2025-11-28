#pragma once
#include "AssetsManager.h"
#include "FileEntity.h"

namespace Zephyrus::Assets
{
    class AssetData
    {
    public:
        std::unique_ptr<FileSystem::FileEntity> mFileEntity;

    public:
        AssetData(const std::string& pFilePath);
        virtual ~AssetData();
    };
}

