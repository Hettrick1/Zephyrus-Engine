#include "pch.h"
#include "AssetData.h"

namespace Zephyrus::Assets
{
    AssetData::AssetData(const std::string& pFilePath)
        : mFileEntity{std::make_unique<FileSystem::FileEntity>(pFilePath)}
    {
        // TODO : LOAD A THUMBNAIL HERE FOR EVERY ASSETS EXECEPT MAPS, SHADERS, FONTS
        // TODO : DO IT IN THE CONTENT BROWSER NOT IN THE RUNTIME
    }

    AssetData::~AssetData()
    {
        if (mFileEntity)
        {
            mFileEntity = nullptr;
        }
    }
}
