#include <iostream>
#include "src/FSManager/FSManager.h"
#include "src/CLI/KVCli.h"
using namespace std;


int main(int argc, char* argv[]) {
    bool baseDirectoryExists = FSManager::createBaseDirectory();
    if(!baseDirectoryExists) return 1;

    return KVCli::handle(argc, argv);
}
