#include "pch.h"
#include "AssetDataBase.h"
#include "ISceneContext.h"
#include "ISerializationFactory.h"

#include "Log.h"

#ifndef UUID_SYSTEM_GENERATOR
    #define UUID_SYSTEM_GENERATOR
#endif
#include "uuid.h"

void AssetDataBase::Init(ISceneContext* context)
{
    mContext = context;
    RefreshContent();
}

void AssetDataBase::RefreshContent()
{
    CheckForFiles(mContentFilePath);
}

void AssetDataBase::RetriveMetaDatas(std::filesystem::path file)
{
    std::filesystem::path metaPath = file.string() + ".meta";
    if (std::filesystem::exists(metaPath))
    {
        //metadata already exist so read from it
        auto reader = mContext->GetSerializationFactory()->CreateDeserializer();

        if (!reader->LoadDocument(metaPath.string()))
        {
            ZP_CORE_ERROR("Impossible to open or parse the prefab: " + metaPath.string());
        }
        
        if (auto id = reader->ReadString("id"))
        {
            UpdatePathFromID(*id, file.string());
        }            
    }
    else
    {
        // metadata don't exist so create the file
        auto writer = mContext->GetSerializationFactory()->CreateSerializer();

        std::string id = uuids::to_string(uuids::uuid_system_generator{}());
        
        writer->WriteString("id", id);
        writer->WriteInt("version", 1.0);
        
        writer->SaveDocument(metaPath.string());

        UpdatePathFromID(id, file.string());
    }
}

void AssetDataBase::CheckForFiles(const std::filesystem::path& pFilePath)
{
    for (auto& entry : std::filesystem::directory_iterator(pFilePath))
    {
        if (entry.is_directory())
        {
            CheckForFiles(entry.path());
        }
        else
        {
            if (entry.path().extension() != ".meta")
            {
                RetriveMetaDatas(entry.path());
                mFiles.emplace_back(entry.path());
            }
        }
    }
}

std::string AssetDataBase::GetPathFromID(const std::string& id)
{
    if (mContent.contains(id))
    {
        return mContent[id];
    }
    ZP_CORE_ERROR("The asset with the id : " + id + " is nowhere to be found. It may have been forced deleted.");
    return "None";
}

void AssetDataBase::UpdatePathFromID(const std::string& id, const std::string& path)
{
    mContent[id] = path;
}
