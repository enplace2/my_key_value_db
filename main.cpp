#include <iostream>
#include "src/FSManager/FSManager.h"
#include "src/CLI/KVCli.h"
#include "src/KVDatabase/KVDatabase.h"
using namespace std;


int main(int argc, char* argv[]) {
    bool baseDirectoryExists = FSManager::createBaseDirectory();
    if(!baseDirectoryExists) return 1;
    std::string dbName = "test_db_2";
    KVDatabase db = KVDatabase::createEmptyDb(dbName);
    std::string directory = db.getDirectory();

    std::cout << "Directory: " << directory << std::endl;

    return 0;

    return KVCli::handle(argc, argv);
}
