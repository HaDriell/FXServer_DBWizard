#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

#include "ResourceManager.h"


int main(int argc, char** argv)
{
    ResourceManager manager;

    if (argc == 2)
    {
        manager.SetResourcePath(argv[1]);
    }
    else
    {
        manager.SetResourcePath("resources");
    }

    manager.LoadResources();
    manager.Debug();

    std::vector<std::string> resourceArray;
    manager.ResolveLoadedResourcesDependencies(resourceArray);

    std::cout << "Loading Order:" << "\n";
    for (const std::string& resourceName : resourceArray)
    {
        std::cout << "\t" << resourceName << "\n";
    }
    
    // Generating the SQL file
    std::ofstream output;
    output.open("output.sql");
    if (output.is_open())
    {
        for (const std::string& resourceName : resourceArray)
        {
            output << "-- Resource : " << resourceName << "\n";

            Resource* resource = manager.GetResource(resourceName);
            for (const std::filesystem::path& file : resource->GetSQLFiles())
            {
                std::cout << "Writing File : " << file << "\n";

                std::ifstream sql;
                sql.open(file);

                if (sql.is_open())
                {
                    std::ostringstream buffer;
                    buffer << sql.rdbuf();
                    output << "\n" << buffer.str() << "\n";
                    sql.close();
                }
            }
        }
        output.close();
    }

    return 0;
}