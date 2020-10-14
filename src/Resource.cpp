#include "Resource.h"

Resource::Resource(const std::string& name)
    : m_Name(name)
{
}

std::string& Resource::GetName()
{
    return m_Name;
}

std::vector<std::string>& Resource::GetProvides()
{
    return m_Provides;
}

std::vector<std::string>& Resource::GetDependencies()
{
    return m_Dependencies;
}
