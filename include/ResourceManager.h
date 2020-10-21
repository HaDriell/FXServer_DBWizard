#pragma once

#include <filesystem>
#include <unordered_map>
#include <string>

#include "Resource.h"

class ResourceManager
{
public:
    ~ResourceManager();

    void SetResourcePath(const std::filesystem::path& path);

    void UnloadResources();
    void LoadResources();
    void LoadFolder(const std::filesystem::path& directory);

    Resource* GetResource(const std::string& resourceName) const;

    bool FindResourceWithNoUnresolvedDependencies(const std::vector<std::string>& availableResources, std::string& resourceName) const;
    bool ResolveLoadedResourcesDependencies(std::vector<std::string>& loadOrder) const;

    void Debug();

private:
    void LoadCategory(const std::filesystem::path& directory);
    void LoadResource(const std::filesystem::path& directory);

private:
    std::filesystem::path m_ResourcePath;
    std::unordered_map<std::string, Resource*> m_Resources;
};