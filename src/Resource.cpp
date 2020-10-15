#include "Resource.h"

#include <iostream>
#include <fstream>

static bool LoadFileInMemory(const std::filesystem::path& file, std::vector<char>& buffer)
{
	//Not a File
    if (!std::filesystem::is_regular_file(file))
    {
        return false;
    }

	std::ifstream stream(file.string(), std::ios::binary | std::ios::ate);

	//File failed to open
	if (!stream.is_open())
	{
        return false;
	}
	
    std::streamsize size = stream.tellg();
    stream.seekg(0, std::ios::beg);

	//File failed to read (WTF ?!)
    buffer.resize(size);
    if (!stream.read(buffer.data(), size))
    {
        return false;
    }
	
    return true;
}

static bool ExpectWord(const std::string& word, const std::vector<char>& buffer, uint64_t position)
{	
	for (uint64_t i = 0; i < word.size(); ++i)
	{
        uint64_t index = position + i;

        //Check BufferOverflow
		if (index >= buffer.size())
        {
            return false;
        }

		//Check character equality
		if (buffer[index] != word[i])
		{
            return false;
		}
	}

    //Make sure that we hit the end of the buffer or that the last character is not alphanumerical
    uint64_t endPosition = position + word.size();
	if (endPosition < buffer.size())
	{
        return !std::isalnum(buffer[endPosition]);
	}
	
    return true;
}

static bool FindWord(const std::string& word, const std::vector<char>& buffer, uint64_t position)
{
	for (uint64_t index = position; index < buffer.size(); ++index)
	{
        if (ExpectWord(word, buffer, index))
        {
            return true;
        }
	}
	
	return false;
}

static std::vector<uint64_t> FindAllWords(const std::string& word, const std::vector<char>& buffer, uint64_t position)
{
    std::vector<uint64_t> indices;

	for (uint64_t index = position; index + word.size() < buffer.size(); ++index)
	{
		if (FindWord(word, buffer, index))
		{
            indices.push_back(index);
		}
	}

    return indices;
}

static bool ParseLUAString(std::string& argument, const std::vector<char>& buffer, uint64_t position)
{
	//Left Trim
    while (std::isspace(buffer[position]))
    {
        ++position;
    }
	
    char stringToken = buffer[position];

	//Expect a String token
	if (stringToken != '\'' && stringToken != '"')
	{
        return false;
	}

	uint64_t begin = position + 1;
    uint64_t end = begin;
	
    //Find the next StringToken
    while (buffer[end] != stringToken)
	{
        ++end;

		//Reaching end of buffer
		if (end >= buffer.size())
		{
            return false;
		}
	}

	//Assign Argument
    argument = std::string(buffer.begin() + begin, buffer.begin() + end);
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

    return true;
}

const std::string& Resource::GetName() const
{
    return m_Name;
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
    std::vector<char> buffer;
	if (!LoadFileInMemory(m_ManifestFile, buffer))
	{
        return false;
	}

	//Reset the Dependencies & Provides declared
    m_Dependencies.clear();
    m_Provides.clear();

    for (uint64_t index : FindAllWords("dependency", buffer, 0))
    {
        std::string name;
        if (ParseLUAString(name, buffer, index + 10))
        {
            m_Dependencies.push_back(name);
        }
    }

    for (uint64_t index : FindAllWords("dependencies", buffer, 0))
    {
        //TODO : Parse a corresponding LUA List of String Arguments for each occurence
    }

    for (uint64_t index : FindAllWords("provide", buffer, 0))
    {
        std::string name;
        if (ParseLUAString(name, buffer, index + 7))
        {
            m_Provides.push_back(name);
        }
    }

    return true;
}