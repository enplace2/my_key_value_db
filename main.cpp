#include <iostream>
#include <filesystem>
#include "src/FSManager/FSManager.h"
#include "src/CLI/KVCli.h"
#include "src/KVDatabase/KVDatabase.h"
using namespace std;


int main(int argc, char* argv[]) {
    bool baseDirectoryExists = FSManager::createBaseDirectory();
    if(!baseDirectoryExists) return 1;

    std::string dbName = "default";
    std::string dbPath = FSManager::getDbStoreFilePath(dbName);

    KVDatabase db = std::filesystem::exists(dbPath)
        ? KVDatabase::load(dbName)
        : KVDatabase::createEmptyDb(dbName);

    KVCli::runInteractiveMode(db);

    return 0;
}
