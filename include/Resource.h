#pragma once

#include <filesystem>
#include <string>
#include <vector>

class Resource
{
public:
    bool Load(const std::filesystem::path& directory);

private:
    bool FindManifestFile();
    bool ParseManifestFile();

private:
    std::filesystem::path m_Directory;
    std::filesystem::path m_ManifestFile;
    std::filesystem::path m_SQLFile;

    std::string m_Name;

    std::vector<std::string> m_Provides;
    std::vector<std::string> m_Dependencies;
};