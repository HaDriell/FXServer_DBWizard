#include "ResourceManager.h"

#include <iostream>

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
