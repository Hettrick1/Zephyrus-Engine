#include "pch.h"
#include "AssetDataBase.h"
#include "ISceneContext.h"
#include "ISerializationFactory.h"

#include "Log.h"

void AssetDataBase::Init(ISceneContext* context)
{
    mContext = context;
    RefreshContent();
}

void AssetDataBase::RefreshContent()
{
    CheckForFiles(mContentFilePath);
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
            auto serializer = mContext->GetSerializationFactory()->CreateSerializer();

            serializer->WriteString("id :", "testID");
            serializer->WriteInt("version :", 1.0);

            serializer->SaveDocument(entry.path().string() + entry.path().filename().string() + ".meta" + entry.path().extension().string());
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
