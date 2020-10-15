#include "Resource.h"

#include <iostream>
#include <fstream>

bool Resource::Load(const std::filesystem::path& directory)
{
    if (!std::filesystem::is_directory(directory))
    {
        return false;
    }

    m_Directory = directory;
    if (!FindManifestFile())
    {
        return false;
    }

    if (!ParseManifestFile())
    {
        return false;
    }

    return true;
}

bool Resource::FindManifestFile()
{
    //Old Manifest name
    if (std::filesystem::is_regular_file(m_Directory / "__resource.lua"))
    {
        m_ManifestFile = m_Directory / "__resource.lua";
        return true;
    }

    //New Manifest name
    if (std::filesystem::is_regular_file(m_Directory / "fxmanifest.lua"))
    {
        m_ManifestFile = m_Directory / "fxmanifest.lua";
        return true;
    }

    //Failed to find the Manifest file
    m_ManifestFile = "";
    return false;
}

bool Resource::ParseManifestFile()
{
    std::cout << "TODO : Parse the Manifest File to load Provides & Dependencies " << "\n";
    return true;
}

/*

std::filesystem::path ResourceManager::FindSQLFile(const std::filesystem::path& directory)
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

*/