#pragma once

#include <string>
#include <vector>

class Resource
{
public:
    Resource(const std::string& name);

    std::string& GetName();
    std::vector<std::string>& GetProvides();
    std::vector<std::string>& GetDependencies();

private:
    std::string m_Name;
    std::vector<std::string> m_Provides;
    std::vector<std::string> m_Dependencies;
};