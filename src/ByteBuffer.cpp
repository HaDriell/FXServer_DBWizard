#include "ByteBuffer.h"

#include <fstream>

bool ByteBuffer::Load(const std::filesystem::path& file)
{
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
    m_Buffer.resize(size);
    m_Position = 0;
    
    if (!stream.read(m_Buffer.data(), size))
    {
        return false;
    }

    return true;
}

bool ByteBuffer::HasRemainingBytes(uint64_t size) const
{
    return m_Position + size <= m_Buffer.size();
}

bool ByteBuffer::Skip(uint64_t amount)
{
    if (HasRemainingBytes(amount))
    {
        m_Position += amount;
        return true;
    }

    return false;
}

bool ByteBuffer::SkipUntilMatchToken(const std::string& token)
{
    do {
        if (MatchToken(token))
        {
            return true;
        }
    } while (Skip(1));

    return false;
}

bool ByteBuffer::SkipWhitespaces()
{
    while (IsWhitespace())
    {
        if (!Skip(1))
        {
            return false;
        }
    }

    return true;
}

bool ByteBuffer::MatchToken(const std::string& token)
{
    if (!HasRemainingBytes(token.size()))
    {
        return false;
    }

    for (uint64_t i = 0; i < token.size(); ++i)
    {
        if (token[i] != m_Buffer[m_Position + i])
        {
            return false;
        }
    }

    m_Position += token.size();
    return true;
}

bool ByteBuffer::IsWhitespace()
{
    return std::isspace(m_Buffer[m_Position]);
}

std::string ByteBuffer::GetSubstring(uint64_t begin, uint64_t end) const
{
    //Yolo on errors
    return std::string(&m_Buffer[begin], &m_Buffer[end]);
}