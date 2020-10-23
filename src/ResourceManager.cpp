#include "ResourceManager.h"

#include <iostream>
#include <algorithm>

ResourceManager::~ResourceManager()
{
    UnloadResources();
}

void ResourceManager::SetResourcePath(const std::filesystem::path& path)
{
    m_ResourcePath = path;
}

void ResourceManager::UnloadResources()
{
    for (const auto& pair : m_Resources)
    {
        delete pair.second;
    }
    m_Resources.clear();
}

void ResourceManager::LoadResources()
{
    UnloadResources();
    for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(m_ResourcePath))
    {
        LoadFolder(entry.path());
    }
}

void ResourceManager::LoadFolder(const std::filesystem::path& directory)
{
    if (std::filesystem::is_directory(directory))
    {
        if (directory.filename().string()[0] == '[')
        {
            LoadCategory(directory);
        }
        else
        {
            LoadResource(directory);
        }
    }
}

Resource* ResourceManager::GetResource(const std::string& resourceName) const
{
    const auto& it = m_Resources.find(resourceName);
    return it != m_Resources.end() ? it->second : nullptr;
}

bool ResourceManager::FindResourceWithNoUnresolvedDependencies(const std::vector<std::string>& availableResources, std::string& resourceName) const
{
    for (const auto& entry : m_Resources)
    {
        //Ignore Resource if it's name is already available
        if (std::find(availableResources.begin(), availableResources.end(), entry.first) != availableResources.end())
        {
            continue;
        }

        //Ignore Resource if there is at least one of it's dependencies unavailable
        bool hasUnresolvedDependencies = false;
        for (const std::string& dependencyName : entry.second->GetDependencies())
        {
            if (std::find(availableResources.begin(), availableResources.end(), dependencyName) == availableResources.end())
            {
                hasUnresolvedDependencies = true;
                break;
            }
        }
        if (hasUnresolvedDependencies)
        {
            continue;
        }

        //This Resource is eligible
        resourceName = entry.first;
        return true;
    }

    return false;
}

bool ResourceManager::ResolveLoadedResourcesDependencies(std::vector<std::string>& loadOrder) const
{
    std::vector<std::string> availableResources;
    std::string resourceName;
    while (FindResourceWithNoUnresolvedDependencies(availableResources, resourceName))
    {
        const auto& it = m_Resources.find(resourceName);

        //This Resource can be loaded now
        loadOrder.push_back(it->first);

        //Append the provided resources to the list of available resources
        availableResources.push_back(resourceName);
        for (const std::string& providedResourceName : it->second->GetProvides())
        {
            availableResources.push_back(providedResourceName);
        }
    }

    //We failed if loadOrder does not include exaclty the same amount of resources from Resources
    return loadOrder.size() == m_Resources.size();
}

void ResourceManager::Debug()
{
    std::cout << "Loaded Resources:" << "\n";
	for (const auto& pair : m_Resources)
	{
        std::cout << "\n";
        pair.second->Debug();
	}
}

void ResourceManager::LoadCategory(const std::filesystem::path& directory)
{
    for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(directory))
    {
        LoadFolder(entry.path());
    }
}

void ResourceManager::LoadResource(const std::filesystem::path& directory)
{
    Resource* resource = new Resource();
    if (!resource->Load(directory))
    {
        std::cout << "[FAIL] Resource at : " << directory << "\n";
        delete resource;
        return;
    }

    auto it = m_Resources.find(resource->GetName());
    if (it != m_Resources.end())
    {
        std::cout << "[FAIL] Duplicate Resource : " << resource->GetName() << "\n";
        delete resource;
        return;
    }

    std::cout << "[LOAD] Resource : " << resource->GetName() << "\n";
    m_Resources.insert(std::make_pair(resource->GetName(), resource));
}