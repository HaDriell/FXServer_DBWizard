#pragma once

#include <filesystem>
#include <string>
#include <vector>

struct Resource
{
    std::filesystem::path m_Directory;
    std::filesystem::path m_ManifestFile;
    std::filesystem::path m_SQLFile;

    std::vector<std::string> m_Provides;
    std::vector<std::string> m_Dependencies;
};