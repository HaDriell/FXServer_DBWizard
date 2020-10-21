#pragma once

#include <filesystem>
#include <string>
#include <istream>

class ByteBuffer
{
public:
    bool Load(const std::filesystem::path& file);
    bool HasRemainingBytes(uint64_t size = 1) const;

    bool Skip(uint64_t amount);
    bool SkipUntilMatchToken(const std::string& token);
    bool SkipWhitespaces();

    bool MatchToken(const std::string& token);
    bool IsWhitespace();

    std::string GetSubstring(uint64_t begin, uint64_t end) const;

    inline void SetPosition(uint64_t position) { m_Position = position; }
    inline uint64_t GetPosition() const { return m_Position; }

private:
    std::vector<char> m_Buffer;
    uint64_t m_Position;
};