#include "ResourceManager.h"

#include <iostream>


void ResourceManager::SetResourcePath(const std::filesystem::path& path)
{
    m_ResourcePath = path;
}

void ResourceManager::ParseResources()
{
    for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(m_ResourcePath))
    {
        ParseResource(entry.path());
    }
}

void ResourceManager::ParseResource(const std::filesystem::path& path)
{
    if (std::filesystem::is_directory(path))
    {
        if (path.filename().string()[0] == '[')
        {
            for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(path))
            {
                ParseResource(entry.path());
            }
        }
        else 
        {
            // This is a Resource Folder
            std::cout << "Resource Folder Found : " << path << std::endl;
        }
    }
}
