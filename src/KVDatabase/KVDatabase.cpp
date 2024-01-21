//
// Created by Nick Place on 1/15/24.
//

#include "KVDatabase.h"
#include "../FSManager/FSManager.h"
#include <filesystem>

namespace fs = std::filesystem;

KVDatabase::KVDatabase(const std::string& dbName, const std::string& fullPath) {
    this->name = dbName;
    this->fullPath = fullPath;
}

KVDatabase::~KVDatabase()  = default;

std::string KVDatabase::getDirectory() {
    return this->fullPath;
}
KVDatabase KVDatabase::createEmptyDb(std::string &dbName) {

    std::string directoryPath = FSManager::createDBDirectory(dbName);
    return KVDatabase(dbName, directoryPath);
}

void KVDatabase::destroy() {
    if(fs::exists(this->fullPath)){
        fs::remove_all(this->fullPath);
    }
}

std::string KVDatabase::store(std::string &key, std::string &value) {
    std::ofstream os = FSManager::openDBWriteStream(this->fullPath, key);
    os << value;
    os.close();
    return value;

}

std::string KVDatabase::get(std::string &key){
    return "Hello";
}

