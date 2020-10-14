#pragma once

#include <filesystem>
#include <unordered_map>
#include <string>

#include "Resource.h"


class ResourceManager
{
public:
    void SetResourcePath(const std::filesystem::path& path);

    void ParseResources();
    void ParseResourceFolder(const std::filesystem::path& path);

    bool FindManifestFile(const std::filesystem::path& directory, std::filesystem::path& file);
    bool FindSQLFile(const std::filesystem::path& directory, std::filesystem::path& file);
    void ParseManifestFile(Resource& resource);

private:
    std::filesystem::path m_ResourcePath;
    std::unordered_map<std::string, Resource> m_Resources;
};