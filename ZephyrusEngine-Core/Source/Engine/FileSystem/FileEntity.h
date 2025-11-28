#pragma once

#include <filesystem>
#include <string>

namespace Zephyrus::FileSystem
{
    class FileEntity
    {
        private:
            std::string mName;
            std::string mExtension;
            std::filesystem::path mPath;
            std::string mPathString;
        public:
        FileEntity(const std::string& path);
        ~FileEntity() = default;

        void SetName(const std::string& name) { mName = name; }
        void SetExtension(const std::string& extension) { mExtension = extension; }
        void SetPath(const std::string& path)
        {
            mPath = std::filesystem::path(path);
            mPathString = path;
            mName = mPath.filename().string();
            mExtension = mPath.extension().string();
        }
        std::string GetName() const { return mName; }
        std::string GetExtension() const { return mExtension; }
        std::string GetPathString() const { return mPathString; }
        std::filesystem::path GetPath() const { return mPath; }
    };
}

