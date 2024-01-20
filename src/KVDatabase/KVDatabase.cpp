//
// Created by Nick Place on 1/15/24.
//

#include "KVDatabase.h"


KVDatabase::KVDatabase(const std::string& dbName, const std::string& fullPath) {
    this->name = dbName;
    this->fullPath = fullPath;
}

KVDatabase::~KVDatabase()  = default;

KVDatabase KVDatabase::createEmptyDb(std::string &dbName) {
    std::string baseDir = "my_kv_db";

    return KVDatabase{dbName, dbName};
}

std::string KVDatabase::getDirectory(void) {
    return std::string();
}
