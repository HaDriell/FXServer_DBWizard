#pragma once

#include <filesystem>
#include <string>
#include <vector>

class Resource
{
public:
    bool Load(const std::filesystem::path& directory);

    const std::string& GetName() const;

    void Debug() const;

private:
    bool FindManifestFile();
    bool ParseManifestFile();
	
private:
    std::string m_Name;

	std::filesystem::path m_Directory;
    std::filesystem::path m_ManifestFile;

    std::vector<std::filesystem::path> m_SQLFiles;
    std::vector<std::string> m_Provides;
    std::vector<std::string> m_Dependencies;
};