
#include <iostream>
#include <stdlib.h>
#include <limits.h>

int main()
{
    std::string root = "./data";
    std::string uri = "/../../";
    std::string realPath;
    std::string _realPath = root + uri;
    char resolved[PATH_MAX];
    if (realpath(_realPath.c_str(), resolved) == NULL)
    {
        std::cerr << "Error resolving path: " << std::endl;
        return 1;
    }
    std::cout << "Resolved path: " << resolved << std::endl;
    // std::string validPath(resolved);
    // if (validPath.find(root) != 0)
    // {
    //     std::cerr << "Error: resolved path is outside of root directory." << std::endl;
    //     return 1;
    // }
    // realPath = validPath;
    // std::cout << "Resolved path: " << realPath << std::endl;

    return 0;
}