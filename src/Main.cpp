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

    manager.ParseResources();

    return 0;
}