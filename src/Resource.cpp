#include "Resource.h"

#include <iostream>
#include <fstream>

#include "ByteBuffer.h"

static bool __ReadStringWithDelimiter(ByteBuffer& buffer, const std::string& delimiter, std::string& value)
{
    if (!buffer.SkipWhitespaces())
    {
        return false;
    }

    if (!buffer.MatchToken(delimiter))
    {
        return false;
    }

    uint64_t begin = buffer.GetPosition();

    if (!buffer.SkipUntilMatchToken(delimiter))
    {
        return false;
    }

    uint64_t end = buffer.GetPosition() - 1;

    value = buffer.GetSubstring(begin, end);

    return true;
}

static bool __ReadLUAString(ByteBuffer& buffer, std::string& value)
{
    uint64_t position = buffer.GetPosition();
    if (__ReadStringWithDelimiter(buffer, "\"", value))
    {
        return true;
    }

    buffer.SetPosition(position);
    if (__ReadStringWithDelimiter(buffer, "'", value))
    {
        return true;
    }

    //Reset position in case of failure
    buffer.SetPosition(position);
    return false;
}

static bool __ReadLUAStringArray(ByteBuffer& buffer, std::vector<std::string>& array)
{
    //Trim
    if (!buffer.SkipWhitespaces())
    {
        return false;
    }

    //Expect Array beginning
    if (!buffer.MatchToken("{"))
    {
        return false;
    }

    while (buffer.HasRemainingBytes())
    {
        //Read LUAString
        std::string entry;
        if (!__ReadLUAString(buffer, entry))
        {
            break;
        }

        array.push_back(entry);

        //Trim
        if (!buffer.SkipWhitespaces())
        {
            break;
        }

        //Expect Array element separator
        if (buffer.MatchToken(","))
        {
            continue;
        }

        //Expect end of array
        if (buffer.MatchToken("}"))
        {
            break;
        }
    }

    return true;
}

bool Resource::Load(const std::filesystem::path& directory)
{
    if (!std::filesystem::is_directory(directory))
    {
        return false;
    }

    m_Name = directory.filename().string();
    m_Directory = directory;
	
    if (!FindManifestFile())
    {
        return false;
    }

    if (!ParseManifestFile())
    {
        return false;
    }

    FindSQLFiles();

    return true;
}

const std::string& Resource::GetName() const
{
    return m_Name;
}

const std::vector<std::string>& Resource::GetDependencies() const
{
    return m_Dependencies;
}

const std::vector<std::string>& Resource::GetProvides() const
{
    return m_Provides;
}

const std::vector<std::filesystem::path>& Resource::GetSQLFiles() const
{
    return m_SQLFiles;
}

void Resource::Debug() const
{
    std::cout << "Resource : " << m_Name << "\n";
    std::cout << "Directory: " << m_Directory << "\n";
    std::cout << "SQL Files: " << m_SQLFiles.size() << "\n";

	std::cout << "Dependencies :\n";
	for (const std::string& name : m_Dependencies)
	{
        std::cout << "\t" << name << "\n";
	}

    std::cout << "Providing :\n";
	for (const std::string& name : m_Provides)
	{
        std::cout << "\t" << name << "\n";
	}
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
    ByteBuffer buffer;

    //Read the Manifest File
    if (!buffer.Load(m_ManifestFile))
    {
        return false;
    }

	//Reset the Dependencies & Provides declared
    m_Dependencies.clear();
    m_Provides.clear();

    //Token pass : dependency
    buffer.SetPosition(0);
    while (buffer.SkipUntilMatchToken("dependency"))
    {
        std::string resourceName;
        if (__ReadLUAString(buffer, resourceName))
        {
            m_Dependencies.push_back(resourceName);
        }
    }

    //Token pass : dependencies
    buffer.SetPosition(0);
    while (buffer.SkipUntilMatchToken("dependencies"))
    {
        if (!__ReadLUAStringArray(buffer, m_Dependencies))
        {
            break;
        }
    }

    //Token pass : provide
    buffer.SetPosition(0);
    while (buffer.SkipUntilMatchToken("provide"))
    {
        std::string resourceName;
        if (__ReadLUAString(buffer, resourceName))
        {
            m_Provides.push_back(resourceName);
        }
    }

    return true;
}

void Resource::FindSQLFiles()
{
    m_SQLFiles.clear();
    for (const auto& entry : std::filesystem::directory_iterator(m_Directory))
    {
        std::filesystem::path file = entry.path();
        if (!std::filesystem::is_regular_file(file))
        {
            continue;
        }

        if (file.extension() != ".sql")
        {
            continue;
        }

        m_SQLFiles.push_back(file);
    }
}
