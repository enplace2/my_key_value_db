#include <iostream>
#include "src/FSManager/FSManager.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    bool baseDirectoryExists = FSManager::createBaseDirectory();
    if(!baseDirectoryExists) return 1;
    return 0;
}
