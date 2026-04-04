#pragma once

#include <filesystem>
#include <unordered_map>
#include <string>
#include <vector>

class ISceneContext;

class AssetDataBase
{
private:
    const std::filesystem::path mContentFilePath = "../Content";
    std::unordered_map<std::string, std::string> mContent;
    //std::vector<std::filesystem::path> mMetaFiles;
    std::vector<std::filesystem::path> mFiles;
    ISceneContext* mContext{nullptr};

    std::unordered_map<std::string, std::string> mContentTEMP;
public:
    void Init(ISceneContext* context);
    void RefreshContent();
    void RetriveMetaDatas(std::filesystem::path file);
    void CheckForFiles(const std::filesystem::path& pFilePath);
    std::string GetPathFromID(const std::string& id);
    void UpdatePathFromID(const std::string& id, const std::string& path);
    std::string CreateMetaFromFile(const std::filesystem::path& pFilePath); // return the new asset id

    const std::unordered_map<std::string, std::string>& GetContent() { return mContent; }
    
    // TEMP
    std::string GetIdFromPath(const std::string& path);
};
