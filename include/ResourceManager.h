#pragma once

#include <filesystem>
#include <vector>
#include <string>

#include "Resource.h"


class ResourceManager
{
public:
    Resource& CreateResource(const std::string& name);

    void SetResourcePath(const std::filesystem::path& path);

    void ParseResources();
    void ParseResource(const std::filesystem::path& path);

private:
    std::filesystem::path m_ResourcePath;

    std::vector<Resource> m_Resources;
};