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
    if (resource->Load(directory))
    {
        m_Resources.insert_or_assign("name", resource);

        std::cout << "[LOAD] ResourcePath : " << directory << "\n";
    }
    else
    {
        std::cout << "[FAIL] ResourcePath : " << directory << "\n";
        delete resource;
    }
}