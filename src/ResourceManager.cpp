#include "ResourceManager.h"

#include <iostream>
#include <fstream>

void ResourceManager::SetResourcePath(const std::filesystem::path& path)
{
    m_ResourcePath = path;
}

void ResourceManager::ParseResources()
{
    for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(m_ResourcePath))
    {
        ParseResourceFolder(entry.path());
    }
}

void ResourceManager::ParseResourceFolder(const std::filesystem::path& path)
{
    if (std::filesystem::is_directory(path))
    {
        if (path.filename().string()[0] == '[')
        {
            for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(path))
            {
                ParseResourceFolder(entry.path());
            }
        }
        else 
        {
            // This is a Resource Folder
            std::cout << "Resource Folder Found : " << path << std::endl;

            Resource resource;
            resource.m_Directory = path;

            //Finding the manifest is mandatory to register the Resource as valid
            if (FindManifestFile(resource.m_Directory, resource.m_ManifestFile))
            {
                ParseManifestFile(resource);
                FindSQLFile(resource.m_Directory, resource.m_SQLFile);
            }
        }
    }
}

bool ResourceManager::FindManifestFile(const std::filesystem::path& directory, std::filesystem::path& file)
{
    //Old Manifest name
    if (std::filesystem::is_regular_file(directory / "__resource.lua"))
    {
        file = directory / "__resource.lua";
        return true;
    }

    //New Manifest name
    if (std::filesystem::is_regular_file(directory / "fxmanifest.lua"))
    {
        file = directory / "fxmanifest.lua";
        return true;
    }

    return false;
}

bool ResourceManager::FindSQLFile(const std::filesystem::path& directory, std::filesystem::path& file)
{
    for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(directory))
    {
        if (entry.path().extension().string() == "sql")
        {
            file = entry.path();
            return true;
        }
    }

    return false;
}

void ResourceManager::ParseManifestFile(Resource& resource)
{
    std::ifstream stream(resource.m_ManifestFile);
    if (stream.is_open())
    {
        //TODO : parse the Manifest file
        // - Find all dependencies
        // - Find all provides
    }
}
