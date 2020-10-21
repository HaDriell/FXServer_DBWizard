#pragma once

#include <filesystem>
#include <string>
#include <vector>

class Resource
{
public:
    bool Load(const std::filesystem::path& directory);

    const std::string& GetName() const;
    const std::vector<std::string>& GetDependencies() const;
    const std::vector<std::string>& GetProvides() const;
    const std::vector<std::filesystem::path>& GetSQLFiles() const;

    void Debug() const;

private:
    bool FindManifestFile();
    bool ParseManifestFile();
    void FindSQLFiles();
	
private:
    std::string m_Name;

	std::filesystem::path m_Directory;
    std::filesystem::path m_ManifestFile;

    std::vector<std::filesystem::path> m_SQLFiles;
    std::vector<std::string> m_Provides;
    std::vector<std::string> m_Dependencies;
};