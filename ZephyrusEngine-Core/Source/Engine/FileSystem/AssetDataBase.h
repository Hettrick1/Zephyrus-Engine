#pragma once

#include <filesystem>
#include <unordered_map>
#include <string>

class ISceneContext;

class AssetDataBase
{
private:
    const std::filesystem::path mContentFilePath = "../Content";
    std::unordered_map<std::string, std::string> mContent;
    ISceneContext* mContext{nullptr};
public:
    void Init(ISceneContext* context);
    void RefreshContent();
    void CheckForFiles(const std::filesystem::path& pFilePath);
    std::string GetPathFromID(const std::string& id);
    void UpdatePathFromID(const std::string& id, const std::string& path);
};
